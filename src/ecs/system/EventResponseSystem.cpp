//
// Created by antho on 2026-03-08.
//

#include "EventResponseSystem.h"

#include <list>

#include "Game.h"
#include "World.h"
#include "ERS/OnDestroyEvent.h"

EventResponseSystem::EventResponseSystem(World &world) {
    // Subscriptions
    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::Collision) return;
            const auto& collision = static_cast<const CollisionEvent&>(e); // Cast base type to collision type

            onCollision(collision, "Item", world);
            onCollision(collision, "Wall", world);
            onCollision(collision, "Projectile", world);
            onCollision(collision, "Ladder", world);
            onCollision(collision, "Camera Bounds", world);
            onCollision(collision, "Enemy", world);
            onCollision(collision, "Death", world);
        }
    );

    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::PlayerAction) return;
            const auto& playerAction = static_cast<const PlayerActionEvent&>(e);
            // TODO onPlayerAction
        }
    );

    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::Destroyed) return;
            const auto& destroyed = static_cast<const DestroyedEvent&>(e);
            OnDestroyEvent::onDestroy(destroyed, world);
        }
    );
}

void EventResponseSystem::onCollision(const CollisionEvent& e, const char* otherTag, World& world) {
    Entity* player = nullptr;
    Entity* other = nullptr;

    if (!getCollisionEntities(e, otherTag, player, other)) return;

    if (std::string(otherTag) == "Item") {
        if (e.state != CollisionState::Enter) return;
        if (player->hasComponent<PlayerGroundCheck>()) return;
        if (player->hasComponent<ProjectileTag>()) return;

        other->destroy();

        for (auto& entity: world.getEntities()) {
            if (!entity->hasComponent<SceneState>()) continue;

            // Scene State
            auto& sceneState = entity->getComponent<SceneState>();
        }
    }

    else if (std::string(otherTag) == "Wall") {
        if (player->hasComponent<ProjectileTag>()) return;
        if (e.state == CollisionState::Stay) {
            if (player->hasComponent<PlayerGroundCheck>()) return;

            // Player components
            auto& t = player->getComponent<Transform>();
            auto& v = player->getComponent<Velocity>();
            auto& isGrounded = player->getComponent<IsGrounded>();
            auto& gravity = player->getComponent<Gravity>();
            auto& ladderClimbing = player->getComponent<LadderClimbing>();

            // Colliders
            auto& playerCollider = player->getComponent<Collider>().rect;
            auto& wallCollider = other->getComponent<Collider>().rect;
            float xOffset = player->getComponent<Collider>().xOffset;
            float yOffset = player->getComponent<Collider>().yOffset;

            // Added / Subtracted from position to prevent unnecessary collisions
            float positionOffset = .1f;

            if (e.axis == CollisionAxis::Horizontal) {
                if (ladderClimbing.isClimbing) return;

                float leftPenetrationDepth = playerCollider.x - (wallCollider.x + wallCollider.w);
                float rightPenetrationDepth = (playerCollider.x + playerCollider.w) - wallCollider.x;
                if (std::abs(leftPenetrationDepth) < std::abs(rightPenetrationDepth)) {
                    t.position.x = (wallCollider.x + wallCollider.w + positionOffset - xOffset);
                }
                else {
                    t.position.x = (wallCollider.x - playerCollider.w - positionOffset - xOffset);
                }
                playerCollider.x = t.position.x + xOffset;

            }

            else if (e.axis == CollisionAxis::Vertical) {
                float topPenetrationDepth = playerCollider.y - (wallCollider.y + wallCollider.h);
                float bottomPenetrationDepth = (playerCollider.y + playerCollider.h) - wallCollider.y;

                if (ladderClimbing.isClimbing) {
                    auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
                    if (std::abs(bottomPenetrationDepth) < std::abs(topPenetrationDepth)) {
                        t.position.y = wallCollider.y - playerCollider.h - positionOffset - yOffset;
                        playerCollider.y = t.position.y + yOffset;
                        isGrounded.grounded = true;
                        ladderClimbing.isClimbing = false;
                        gravity.gravityEnabled = false;
                        v.ySpeed = 0;
                        return;
                    }

                    else if (wallCollider.y + (ladderCollider.y - wallCollider.y) < playerCollider.y) return;

                    else if (ladderCollider.y > playerCollider.y) {
                        t.position.y = wallCollider.y - playerCollider.h - positionOffset - yOffset;
                        playerCollider.y = t.position.y + yOffset;
                        isGrounded.grounded = true;
                        ladderClimbing.isClimbing = false;
                        gravity.gravityEnabled = false;
                        playerCollider.y = t.position.y;
                        v.ySpeed = 0;
                        return;
                    }
                }

                v.ySpeed = 0;
                if (std::abs(bottomPenetrationDepth) < std::abs(topPenetrationDepth)) {
                    t.position.y = wallCollider.y - playerCollider.h - positionOffset - yOffset;
                    isGrounded.grounded = true;
                    world.getAudioEventQueue().push(std::make_unique<AudioEvent>("megamanLand"));
                    ladderClimbing.isClimbing = false;
                    gravity.gravityEnabled = false;
                }
                else {
                    t.position.y = (wallCollider.y + wallCollider.h + positionOffset - yOffset);
                }
                playerCollider.y = t.position.y + yOffset;
            }
        }

        if (e.state == CollisionState::Exit) {

            if (player->hasComponent<PlayerGroundCheck>()) {
                auto& isGrounded = player->getComponent<FollowEntity>().followedEntity.getComponent<IsGrounded>();
                auto& gravity = player->getComponent<FollowEntity>().followedEntity.getComponent<Gravity>();
                isGrounded.grounded = false;
                gravity.gravityEnabled = true;
            }
        }
    }

    else if (std::string(otherTag) == "Ladder") {
        if (e.state == CollisionState::Enter) return;
        if (player->hasComponent<PlayerGroundCheck>()) return;

        auto& ladderClimbing = player->getComponent<LadderClimbing>();
        if (player->hasComponent<ProjectileTag>()) return;
        if (e.state == CollisionState::Stay) {
            ladderClimbing.canClimb = true;
            ladderClimbing.ladderEntity = other;

            if (e.axis == CollisionAxis::Horizontal) return;

            auto& playerCollider = player->getComponent<Collider>().rect;
            auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
            float yOffset = player->getComponent<Collider>().yOffset;

            auto& v = player->getComponent<Velocity>();
            auto& t = player->getComponent<Transform>();
            auto& gravity = player->getComponent<Gravity>();
            auto& isGrounded = player->getComponent<IsGrounded>();

            float ladderColliderTopOffset = 3.0f;
            float positionOffset = .1f;

            if (ladderCollider.y > playerCollider.y) {
                t.position.y = ladderCollider.y + ladderColliderTopOffset - playerCollider.h - positionOffset - yOffset;
                playerCollider.y = t.position.y + yOffset;
                isGrounded.grounded = true;
                ladderClimbing.isClimbing = false;
                gravity.gravityEnabled = false;
                v.ySpeed = 0;
            }

            if (ladderClimbing.isClimbing) {
                isGrounded.grounded = false;
            }
        }

        if (e.state == CollisionState::Exit) {
            auto& gravity = player->getComponent<Gravity>();
            ladderClimbing.canClimb = false;
            ladderClimbing.ladderEntity = nullptr;
        }
    }

    else if (std::string(otherTag) == "Camera Bounds") {
        if (e.state !=CollisionState::Enter) return;
        if (player->hasComponent<ProjectileTag>()) return;
        auto& cameraBounds = other->getComponent<Collider>();

        // Find the camera
        for (auto& e : world.getEntities()) {
            if (e->hasComponent<Camera>()) {
                auto& camera = e->getComponent<Camera>();
                camera.worldX = cameraBounds.rect.x;
                camera.worldY = cameraBounds.rect.y;
                camera.worldWidth = cameraBounds.rect.w;
                camera.worldHeight = cameraBounds.rect.h;
                break;
            }
        }
    }


    else if (std::string(otherTag) == "Projectile") {
        if (e.state !=CollisionState::Enter) return;
        if (player->hasComponent<PlayerGroundCheck>()) return;

        /*
        if (player->hasComponent<ProjectileTag>() &&
            player->hasComponent<ProjectileDamage>() &&
            other->hasComponent<Health>()) {
            auto& projectileDamage = player->getComponent<ProjectileDamage>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(projectileDamage.damage, other);
            return;
        }
        */

        if (player->hasComponent<Health>() &&
            player->hasComponent<Invulnerability>() &&
            player->hasComponent<HitKnockback>() &&
            other->hasComponent<Velocity>() &&
            other->hasComponent<ProjectileDamage>()) {

            auto& invulnerability = player->getComponent<Invulnerability>();
            auto& invulTimer = player->getComponent<InvulnerabilityTimer>();
            if (invulnerability.isInvulnerable) return;

            auto& projectileDamage = other->getComponent<ProjectileDamage>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(projectileDamage.damage, player);

            auto& hitKnockback = player->getComponent<HitKnockback>();
            auto& velocity = other->getComponent<Velocity>();
            hitKnockback.isHitKnockback = true;
            if (velocity.direction.x < 0) {
                hitKnockback.isRightHit = true;
            }
            else {
                hitKnockback.isRightHit = false;
            }
            hitKnockback.timer = hitKnockback.minKnockbackTime;
            invulTimer.timer = invulTimer.invulnerabilityTime;
            invulnerability.isInvulnerable = true;
            return;
        }
    }



    else if (std::string(otherTag) == "Enemy") {
        if (e.state !=CollisionState::Enter) return;
        if (player->hasComponent<PlayerGroundCheck>()) return;

        if (player->hasComponent<ProjectileTag>() &&
            player->hasComponent<ProjectileDamage>() &&
            other->hasComponent<Health>()) {

            auto& projectileDamage = player->getComponent<ProjectileDamage>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(projectileDamage.damage, other);

            OnDestroyEvent::onDestroy(player, world);
            player->destroy();
            return;
        }

        if (player->hasComponent<Health>() &&
            other->hasComponent<ContactDamage>()) {
            auto& invulnerability = player->getComponent<Invulnerability>();
            if (invulnerability.isInvulnerable) return;

            auto& invulTimer = player->getComponent<InvulnerabilityTimer>();

            auto& contactDamage = other->getComponent<ContactDamage>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(contactDamage.damage, player);

            auto& hitKnockback = player->getComponent<HitKnockback>();
            auto& velocity = other->getComponent<Velocity>();
            auto& playerCollider = player->getComponent<Collider>().rect;
            auto& enemyCollider = other->getComponent<Collider>().rect;
            hitKnockback.isHitKnockback = true;

            float leftPenetrationDepth = playerCollider.x - (enemyCollider.x + enemyCollider.w);
            float rightPenetrationDepth = (playerCollider.x + playerCollider.w) - enemyCollider.x;
            if (std::abs(leftPenetrationDepth) < std::abs(rightPenetrationDepth)) {
                hitKnockback.isRightHit = false;
            }
            else {
                hitKnockback.isRightHit = true;
            }
            hitKnockback.timer = hitKnockback.minKnockbackTime;
            invulTimer.timer = invulTimer.invulnerabilityTime;
            invulnerability.isInvulnerable = true;
        }
    }

    else if (std::string(otherTag) == "Death") {
        if (player->hasComponent<PlayerGroundCheck>()) return;
        if (e.state !=CollisionState::Stay) return;

        if (player->hasComponent<Health>()) {
            auto& health = player->getComponent<Health>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(health.maxHealth, player, true);
        }
    }
}


bool EventResponseSystem::getCollisionEntities(
    const CollisionEvent &e,
    const char *otherTag,
    Entity *&player,
    Entity *&other
) {
    if (e.entityA == nullptr || e.entityB == nullptr) return false;

    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>())) return false;

    auto& colliderA = e.entityA->getComponent<Collider>();
    auto& colliderB = e.entityB->getComponent<Collider>();

    if (colliderA.tag == "Player" && colliderB.tag == otherTag) {
        player = e.entityA;
        other = e.entityB;
    }
    else if (colliderA.tag == otherTag && colliderB.tag == "Player") {
        player = e.entityB;
        other = e.entityA;
    }

    return player && other;
}

void EventResponseSystem::onPlayerAction(const PlayerActionEvent &e,
    const std::function<void(Entity *player, PlayerAction action)> &callback) {
    if (e.action == PlayerAction::Attack) {
        return;
    }
}