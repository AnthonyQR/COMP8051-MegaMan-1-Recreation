//
// Created by antho on 2026-04-02.
//

#include "OnPlayerCollisionEvent.h"

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
}

void OnPlayerCollisionEvent::wallCollision(Entity *player, Entity *other, const CollisionEvent &e, const char *otherTag,
    World &world) {
    if (player->hasComponent<ProjectileTag>()) return;
    if (player->hasComponent<PlayerHurtbox>()) return;
        if (e.state == CollisionState::Stay) {
            if (player->hasComponent<PlayerGroundCheck>()) {
                player = &player->getComponent<FollowEntity>().followedEntity;
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
                if (v.ySpeed >= 175.0f) {
                    world.getAudioEventQueue().push(std::make_unique<AudioEvent>("megamanLand"));
                }
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
            if (player->hasComponent<PlayerGroundCheck>()) {
                auto& coyoteTime = player->getComponent<FollowEntity>().followedEntity.getComponent<CoyoteTime>();
                coyoteTime.timer = coyoteTime.duration;
                coyoteTime.isCoyoteTime = true;
            }
        }
}

void OnPlayerCollisionEvent::ladderCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (player->hasComponent<PlayerHurtbox>()) return;
    if (e.state == CollisionState::Enter) return;
    if (player->hasComponent<ProjectileTag>()) return;

        if (e.state == CollisionState::Stay) {
            if (player->hasComponent<PlayerGroundCheck>()) {
                if (player->getComponent<FollowEntity>().followedEntity.getComponent<Transform>().position.y < other->getComponent<Collider>().rect.y) {
                    auto& coyoteTime = player->getComponent<FollowEntity>().followedEntity.getComponent<CoyoteTime>();
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

            // if (ladderCollider.y > playerCollider.y) {
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
                std::cout << "Grounded" << std::endl;
            }

            if (ladderClimbing.isClimbing) {
                isGrounded.grounded = false;
                auto& coyoteTime = player->getComponent<CoyoteTime>();
                coyoteTime.timer = coyoteTime.duration;
                coyoteTime.isCoyoteTime = false;
            }
        }

        if (e.state == CollisionState::Exit) {
            if (player->hasComponent<PlayerGroundCheck>()) {
                auto& coyoteTime = player->getComponent<FollowEntity>().followedEntity.getComponent<CoyoteTime>();
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
    if (player->hasComponent<PlayerHurtbox>()) return;
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
            other->hasComponent<Health>()) {
            if (other->hasComponent<Invulnerability>()) {
                auto& invulnerability =other->getComponent<Invulnerability>();
                if (invulnerability.isInvulnerable) {
                    world.getAudioEventQueue().push(std::make_unique<AudioEvent>("dink"));
                    OnDestroyEvent::onDestroy(player, world);
                    player->destroy();
                    return;
                }
            }

            auto& projectileDamage = player->getComponent<ProjectileDamage>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(projectileDamage.damage, other);
            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("enemyDamage"));

            OnDestroyEvent::onDestroy(player, world);
            player->destroy();
            return;
        }

        if (player->hasComponent<PlayerHurtbox>() &&
            other->hasComponent<ContactDamage>()) {
            player = &player->getComponent<FollowEntity>().followedEntity;
            auto& invulnerability = player->getComponent<Invulnerability>();
            if (invulnerability.isInvulnerable) return;

            auto& invulTimer = player->getComponent<InvulnerabilityTimer>();

            auto& contactDamage = other->getComponent<ContactDamage>();
            auto& damageEntity(world.createEntity());
            damageEntity.addComponent<Damage>(contactDamage.damage, player);
            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("enemyShoot"));

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


    if (player->hasComponent<PlayerHurtbox>() &&
        other->hasComponent<Velocity>() &&
        other->hasComponent<ProjectileDamage>()) {

        player = &player->getComponent<FollowEntity>().followedEntity;
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

void OnPlayerCollisionEvent::deathCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (player->hasComponent<PlayerGroundCheck>()) return;
    if (e.state !=CollisionState::Stay) return;

    if (player->hasComponent<Health>()) {
        auto& health = player->getComponent<Health>();
        auto& damageEntity(world.createEntity());
        damageEntity.addComponent<Damage>(health.maxHealth, player, true);
    }
}

void OnPlayerCollisionEvent::enemyDetectCollision(Entity *player, Entity *other, const CollisionEvent &e,
    const char *otherTag, World &world) {
    if (e.state != CollisionState::Stay) return;
    if (player->hasComponent<ProjectileTag>()) return;
    if (player->hasComponent<PlayerGroundCheck>()) return;
    if (player->hasComponent<PlayerHurtbox>()) return;
    if (!other->hasComponent<OnPlayerDetectCallback>())return;

    other->getComponent<OnPlayerDetectCallback>().callback(other, player);
}
