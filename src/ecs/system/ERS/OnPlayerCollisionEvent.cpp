//
// Created by antho on 2026-04-02.
//

#include "OnPlayerCollisionEvent.h"

#include "Game.h"
#include "OnDestroyEvent.h"

void OnPlayerCollisionEvent::onCollision(Entity *player, Entity *other,
    const CollisionEvent &e, const char *otherTag, World &world) {

    if (std::string(otherTag) == "Wall") {
        wallCollision(player, other, e, otherTag, world);
    }

    else if (std::string(otherTag) == "Ladder") {
        ladderCollision(player, other, e, otherTag, world);
    }

    else if (std::string(otherTag) == "Camera Bounds") {
        cameraBoundsCollision(player, other, e, otherTag, world);
    }

    else if (std::string(otherTag) == "Projectile") {
        projectileCollision(player, other, e, otherTag, world);
    }

    else if (std::string(otherTag) == "Enemy") {
        enemyCollision(player, other, e, otherTag, world);
    }

    else if (std::string(otherTag) == "Death") {
        deathCollision(player, other, e, otherTag, world);
    }

    else if (std::string(otherTag) == "EnemyDetect") {
        enemyDetectCollision(player, other, e, otherTag, world);
    }

    else if (std::string(otherTag) == "Checkpoint") {
        checkpointCollision(player, other, e, otherTag, world);
    }

    else if (std::string(otherTag) == "Item") {
        itemCollision(player, other, e, otherTag, world);
    }
}

void OnPlayerCollisionEvent::wallCollision(Entity *player, Entity *other, const CollisionEvent &e, const char *otherTag,
    World &world) {
    if (player->hasComponent<ProjectileTag>()) return;
    if (player->hasComponent<Hurtbox>()) return;
        if (e.state == CollisionState::Stay) {
            if (player->hasComponent<PlayerGroundCheck>() &&
                player->hasComponent<Parent>()) {
                player = player->getComponent<Parent>().parent;
                auto& ladderClimbing = player->getComponent<LadderClimbing>();
                auto& v = player->getComponent<Velocity>();

                // Colliders
                auto& playerCollider = player->getComponent<Collider>().rect;
                auto& wallCollider = other->getComponent<Collider>().rect;
                float xOffset = player->getComponent<Collider>().xOffset;
                float yOffset = player->getComponent<Collider>().yOffset;

                // Added / Subtracted from position to prevent unnecessary collisions
                float positionOffset = .1f;

                if (ladderClimbing.isClimbing) {
                    auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
                    if (v.ySpeed >= 0 && playerCollider.y + playerCollider.h > ladderCollider.y + ladderCollider.h) {
                        auto& t = player->getComponent<Transform>();
                        auto& isGrounded = player->getComponent<IsGrounded>();
                        auto& gravity = player->getComponent<Gravity>();

                        t.position.y = wallCollider.y - playerCollider.h - positionOffset - yOffset;
                        playerCollider.y = t.position.y + yOffset;
                        isGrounded.grounded = true;
                        ladderClimbing.isClimbing = false;
                        gravity.gravityEnabled = false;
                        v.ySpeed = 0;
                        std::cout<< "Wall Collision" << std::endl;
                    }
                }
                auto& coyoteTime = player->getComponent<CoyoteTime>();
                coyoteTime.timer = coyoteTime.duration;
                coyoteTime.isCoyoteTime = false;
                return;
            }
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

            float leftPenetrationDepth = playerCollider.x - (wallCollider.x + wallCollider.w);
            float rightPenetrationDepth = (playerCollider.x + playerCollider.w) - wallCollider.x;
            float topPenetrationDepth = playerCollider.y - (wallCollider.y + wallCollider.h);
            float bottomPenetrationDepth = (playerCollider.y + playerCollider.h) - wallCollider.y;

            if (ladderClimbing.isClimbing) {
                auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;

                if (wallCollider.y + (ladderCollider.y - wallCollider.y) < playerCollider.y) return;

                if (ladderCollider.y > playerCollider.y) {
                    t.position.y = wallCollider.y - playerCollider.h - positionOffset - yOffset;
                    playerCollider.y = t.position.y + yOffset;
                    isGrounded.grounded = true;
                    ladderClimbing.isClimbing = false;
                    gravity.gravityEnabled = false;
                    playerCollider.y = t.position.y;
                    v.ySpeed = 0;
                    return;
                }
                return;
            }

            // Left Side Collision
            if (std::abs(leftPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(leftPenetrationDepth) < std::abs(topPenetrationDepth) &&
                std::abs(leftPenetrationDepth) < std::abs(bottomPenetrationDepth)) {
                t.position.x = (wallCollider.x + wallCollider.w + positionOffset - xOffset);
                playerCollider.x = t.position.x + xOffset;
                return;
            }

            // Right Side Collision
            if (std::abs(rightPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(rightPenetrationDepth) < std::abs(topPenetrationDepth) &&
                std::abs(rightPenetrationDepth) < std::abs(bottomPenetrationDepth)){
                t.position.x = (wallCollider.x - playerCollider.w - positionOffset - xOffset);
                playerCollider.x = t.position.x + xOffset;
                return;
            }

            // Top Side Collision
            if (std::abs(topPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(topPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(topPenetrationDepth) < std::abs(bottomPenetrationDepth) &&
                v.ySpeed <= 0) {
                v.ySpeed = 0;
                t.position.y = (wallCollider.y + wallCollider.h + positionOffset - yOffset);
                return;
            }

            // Bottom Side Collision
            if (std::abs(bottomPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(bottomPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(bottomPenetrationDepth) < std::abs(topPenetrationDepth) &&
                v.ySpeed >= 0.0f) {
                world.getAudioEventQueue().push(std::make_unique<AudioEvent>("megamanLand"));
                v.ySpeed = 0;
                t.position.y = wallCollider.y - playerCollider.h - positionOffset - yOffset;
                isGrounded.grounded = true;

                ladderClimbing.isClimbing = false;
                gravity.gravityEnabled = false;
                playerCollider.y = t.position.y + yOffset;
                return;
            }
        }

        if (e.state == CollisionState::Exit) {
            if (player->hasComponent<PlayerGroundCheck>() &&
                player->hasComponent<Parent>()) {
                auto& coyoteTime = player->getComponent<Parent>().parent->getComponent<CoyoteTime>();
                coyoteTime.timer = coyoteTime.duration;
                coyoteTime.isCoyoteTime = true;
            }
        }
}

void OnPlayerCollisionEvent::ladderCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (player->hasComponent<Hurtbox>()) return;
    if (e.state == CollisionState::Enter) return;
    if (player->hasComponent<ProjectileTag>()) return;

        if (e.state == CollisionState::Stay) {
            if (player->hasComponent<PlayerGroundCheck>() &&
                player->hasComponent<Parent>()) {
                player = player->getComponent<Parent>().parent;
                if (player->getComponent<Transform>().position.y < other->getComponent<Collider>().rect.y) {
                    auto& coyoteTime = player->getComponent<CoyoteTime>();
                    coyoteTime.timer = coyoteTime.duration;
                    coyoteTime.isCoyoteTime = false;
                }
                return;
            }
            auto& ladderClimbing = player->getComponent<LadderClimbing>();
            ladderClimbing.canClimb = true;
            ladderClimbing.ladderEntity = other;

            auto& playerCollider = player->getComponent<Collider>().rect;
            auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
            float yOffset = player->getComponent<Collider>().yOffset;

            auto& v = player->getComponent<Velocity>();
            auto& t = player->getComponent<Transform>();
            auto& gravity = player->getComponent<Gravity>();
            auto& isGrounded = player->getComponent<IsGrounded>();

            float ladderColliderTopOffset = 3.0f;
            float positionOffset = .1f;

            if ((ladderCollider.y > playerCollider.y && ladderClimbing.isClimbing) ||
                (ladderCollider.y > playerCollider.y &&
                (playerCollider.y + (playerCollider.h / 2) - positionOffset < ladderCollider.y + ladderColliderTopOffset) &&
                v.ySpeed > 0)) {
                if (!ladderClimbing.isClimbing) {
                    world.getAudioEventQueue().push(std::make_unique<AudioEvent>("megamanLand"));
                }
                t.position.y = ladderCollider.y + ladderColliderTopOffset - playerCollider.h - positionOffset - yOffset;
                playerCollider.y = t.position.y + yOffset;
                isGrounded.grounded = true;
                ladderClimbing.isClimbing = false;
                gravity.gravityEnabled = false;
                v.ySpeed = 0;
            }

            if (ladderClimbing.isClimbing) {
                isGrounded.grounded = false;
                auto& coyoteTime = player->getComponent<CoyoteTime>();
                coyoteTime.timer = coyoteTime.duration;
                coyoteTime.isCoyoteTime = false;
            }
        }

        if (e.state == CollisionState::Exit) {
            if (player->hasComponent<PlayerGroundCheck>() &&
                player->hasComponent<Parent>()) {
                auto& coyoteTime = player->getComponent<Parent>().parent->getComponent<CoyoteTime>();
                coyoteTime.timer = coyoteTime.duration;
                coyoteTime.isCoyoteTime = true;
                return;
            }
            auto& coyoteTime = player->getComponent<CoyoteTime>();
            coyoteTime.timer = coyoteTime.duration;
            coyoteTime.isCoyoteTime = true;

            auto& ladderClimbing = player->getComponent<LadderClimbing>();
            ladderClimbing.canClimb = false;
            ladderClimbing.isClimbing = false;
            ladderClimbing.ladderEntity = nullptr;
        }
}

void OnPlayerCollisionEvent::cameraBoundsCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (e.state !=CollisionState::Enter) return;
    if (player->hasComponent<ProjectileTag>()) return;
    if (player->hasComponent<Hurtbox>()) return;
    auto& cameraBounds = other->getComponent<Collider>();

    // Find the camera
    for (auto& entity : world.getEntities()) {
        if (entity->hasComponent<Camera>()) {
            auto& camera = entity->getComponent<Camera>();
            camera.worldX = cameraBounds.rect.x;
            camera.worldY = cameraBounds.rect.y;
            camera.worldWidth = cameraBounds.rect.w;
            camera.worldHeight = cameraBounds.rect.h;
            break;
        }
    }
}

void OnPlayerCollisionEvent::enemyCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (e.state !=CollisionState::Enter) return;
    if (player->hasComponent<PlayerGroundCheck>()) return;

        if (player->hasComponent<ProjectileTag>() &&
            player->hasComponent<ProjectileDamage>() &&
            other->hasComponent<Hurtbox>() &&
            other->hasComponent<Parent>()) {

            other = other->getComponent<Parent>().parent;
            if (!other->hasComponent<Health>()) return;

            auto& projectileDamage = player->getComponent<ProjectileDamage>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(projectileDamage.damage, other, player);
            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("enemyDamage"));
            return;
        }

        if (player->hasComponent<Hurtbox>() &&
            player->hasComponent<Parent>() &&
            other->hasComponent<ContactDamage>()) {
            player = player->getComponent<Parent>().parent;
            if (player == nullptr) return;

            auto& contactDamage = other->getComponent<ContactDamage>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(contactDamage.damage, player, other);
        }
}

void OnPlayerCollisionEvent::projectileCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
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


    if (player->hasComponent<Hurtbox>() &&
        player->hasComponent<Parent>() &&
        other->hasComponent<Velocity>() &&
        other->hasComponent<ProjectileDamage>()) {

        player = player->getComponent<Parent>().parent;
        if (player == nullptr) return;

        auto& projectileDamage = other->getComponent<ProjectileDamage>();
        auto& damageEntity(world.createEntity());
        damageEntity.addComponent<Damage>(projectileDamage.damage, player, other);
        }
}

void OnPlayerCollisionEvent::deathCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (player->hasComponent<PlayerGroundCheck>()) return;
    if (e.state !=CollisionState::Stay) return;

    if (player->hasComponent<Health>()) {
        auto& health = player->getComponent<Health>();
        auto& damageEntity(world.createEntity());
        damageEntity.addComponent<Damage>(health.maxHealth, player, other, true);
    }
}

void OnPlayerCollisionEvent::enemyDetectCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (player->hasComponent<ProjectileTag>()) return;
    if (player->hasComponent<PlayerGroundCheck>()) return;
    if (player->hasComponent<Hurtbox>()) return;

    if (e.state == CollisionState::Enter) {
        if (!other->hasComponent<OnPlayerDetectEnterCallback>()) return;
        other->getComponent<OnPlayerDetectEnterCallback>().callback(other, player);
    }

    else if (e.state == CollisionState::Stay) {
        if (!other->hasComponent<OnPlayerDetectStayCallback>()) return;
        other->getComponent<OnPlayerDetectStayCallback>().callback(other, player);
    }

    else if (e.state == CollisionState::Exit) {
        if (!other->hasComponent<OnPlayerDetectLeaveCallback>()) return;
        other->getComponent<OnPlayerDetectLeaveCallback>().callback(other, player);
    }
}

void OnPlayerCollisionEvent::checkpointCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (e.state != CollisionState::Enter) return;
    if (player->hasComponent<ProjectileTag>()) return;
    if (player->hasComponent<PlayerGroundCheck>()) return;
    if (player->hasComponent<Hurtbox>()) return;

    auto& spawnPoints = world.getMap().checkPointSpawnPoints;
    auto& checkPointCollider = other->getComponent<Collider>().rect;

    for (int i = 0; i < spawnPoints.size(); i++) {
        if (spawnPoints[i].x > checkPointCollider.x &&
            spawnPoints[i].x < checkPointCollider.x + checkPointCollider.w &&
            spawnPoints[i].y > checkPointCollider.y &&
            spawnPoints[i].y < checkPointCollider.y + checkPointCollider.h) {
            Game::gameState.currentCheckpoint = i;
            std::cout << "New Checkpoint" << std::endl;
            return;
        }
    }
}

void OnPlayerCollisionEvent::itemCollision(Entity *player, Entity *other, const CollisionEvent &e, const char *otherTag,
    World &world) {
    if (e.state != CollisionState::Enter) return;
    if (player->hasComponent<ProjectileTag>()) return;
    if (player->hasComponent<PlayerGroundCheck>()) return;
    if (player->hasComponent<Hurtbox>()) return;

    if (other->hasComponent<VictoryItemTag>()) {
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("victoryMusic"));

        Game::gameState.isEnding = true;
        Game::checkSceneState();

        auto& transition(world.createDeferredEntity());
        transition.addComponent<SceneTransitionDelay>(6.5f, "victory");
    }

    else if (other->hasComponent<Healing>()){
        auto& healing(world.createDeferredEntity());
        auto& healAmount = other->getComponent<Healing>().healAmount;
        healing.addComponent<HealOvertime>(healAmount, player);
    }

    else if (other->hasComponent<OneUp>()) {
        Game::gameState.lives++;
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("oneUp"));
    }

    other->destroy();
}
