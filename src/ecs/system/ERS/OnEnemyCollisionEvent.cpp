//
// Created by antho on 2026-04-02.
//

#include "OnEnemyCollisionEvent.h"

void OnEnemyCollisionEvent::onCollision(Entity *enemy, Entity *other,
    const CollisionEvent &e, const char *otherTag, World &world) {

    if (std::string(otherTag) == "Wall") {
        if (!enemy->hasComponent<WallCollidable>()) return;
        if (!enemy->getComponent<WallCollidable>().isCollidable) return;

        // Enemy Components
        auto& transform = enemy->getComponent<Transform>();
        auto& velocity = enemy->getComponent<Velocity>();

        // Colliders
        auto& enemyCollider = enemy->getComponent<Collider>().rect;
        auto& wallCollider = other->getComponent<Collider>().rect;
        float xOffset = enemy->getComponent<Collider>().xOffset;
        float yOffset = enemy->getComponent<Collider>().yOffset;

        // Added / Subtracted from position to prevent unnecessary collisions
        float positionOffset = .1f;

        float leftPenetrationDepth = enemyCollider.x - (wallCollider.x + wallCollider.w);
        float rightPenetrationDepth = (enemyCollider.x + enemyCollider.w) - wallCollider.x;
        float topPenetrationDepth = enemyCollider.y - (wallCollider.y + wallCollider.h);
        float bottomPenetrationDepth = (enemyCollider.y + enemyCollider.h) - wallCollider.y;

        // Left Side Collision
            if (std::abs(leftPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(leftPenetrationDepth) < std::abs(topPenetrationDepth) &&
                std::abs(leftPenetrationDepth) < std::abs(bottomPenetrationDepth)) {
                velocity.xSpeed = 0;
                transform.position.x = (wallCollider.x + wallCollider.w + positionOffset - xOffset);
                enemyCollider.x = transform.position.x + xOffset;
                return;
            }

            // Right Side Collision
            if (std::abs(rightPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(rightPenetrationDepth) < std::abs(topPenetrationDepth) &&
                std::abs(rightPenetrationDepth) < std::abs(bottomPenetrationDepth)){
                velocity.xSpeed = 0;
                transform.position.x = (wallCollider.x - enemyCollider.w - positionOffset - xOffset);
                enemyCollider.x = transform.position.x + xOffset;
                return;
            }

            // Top Side Collision
            if (std::abs(topPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(topPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(topPenetrationDepth) < std::abs(bottomPenetrationDepth) &&
                velocity.direction.y < 0) {
                velocity.ySpeed = 0;
                transform.position.y = (wallCollider.y + wallCollider.h + positionOffset - yOffset);
                return;
            }

            // Bottom Side Collision
            if (std::abs(bottomPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(bottomPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(bottomPenetrationDepth) < std::abs(topPenetrationDepth) &&
                velocity.direction.y > 0) {
                velocity.ySpeed = 0;
                transform.position.y = wallCollider.y - enemyCollider.h - positionOffset - yOffset;
                enemyCollider.y = transform.position.y + yOffset;
                return;
            }
    }
}
