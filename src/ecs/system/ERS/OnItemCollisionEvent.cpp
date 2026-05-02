//
// Created by antho on 2026-05-02.
//

#include "OnItemCollisionEvent.h"

void OnItemCollisionEvent::onCollision(Entity *item, Entity *other,
    const CollisionEvent &e, const char *otherTag, World &world) {
    if (e.state == CollisionState::Exit) return;
     if (std::string(otherTag) == "Wall") {
        if (!item->hasComponent<WallCollidable>()) return;
        if (!item->getComponent<WallCollidable>().isCollidable) return;

        // Item Components
        auto& transform = item->getComponent<Transform>();
        auto& velocity = item->getComponent<Velocity>();

        // Colliders
        auto& itemCollider = item->getComponent<Collider>().rect;
        auto& wallCollider = other->getComponent<Collider>().rect;
        float xOffset = item->getComponent<Collider>().xOffset;
        float yOffset = item->getComponent<Collider>().yOffset;

        // Added / Subtracted from position to prevent unnecessary collisions
        float positionOffset = .1f;

        float leftPenetrationDepth = itemCollider.x - (wallCollider.x + wallCollider.w);
        float rightPenetrationDepth = (itemCollider.x + itemCollider.w) - wallCollider.x;
        float topPenetrationDepth = itemCollider.y - (wallCollider.y + wallCollider.h);
        float bottomPenetrationDepth = (itemCollider.y + itemCollider.h) - wallCollider.y;

        // Left Side Collision
            if (std::abs(leftPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(leftPenetrationDepth) < std::abs(topPenetrationDepth) &&
                std::abs(leftPenetrationDepth) < std::abs(bottomPenetrationDepth)) {
                transform.position.x = (wallCollider.x + wallCollider.w + positionOffset - xOffset);
                itemCollider.x = transform.position.x + xOffset;

                if (item->hasComponent<OnLeftCollisionCallback>()) {
                    item->getComponent<OnLeftCollisionCallback>().callback(item);
                }
                return;
            }

            // Right Side Collision
            if (std::abs(rightPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(rightPenetrationDepth) < std::abs(topPenetrationDepth) &&
                std::abs(rightPenetrationDepth) < std::abs(bottomPenetrationDepth)){
                transform.position.x = (wallCollider.x - itemCollider.w - positionOffset - xOffset);
                itemCollider.x = transform.position.x + xOffset;

                if (item->hasComponent<OnRightCollisionCallback>()) {
                    item->getComponent<OnRightCollisionCallback>().callback(item);
                }
                return;
            }

            // Top Side Collision
            if (std::abs(topPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(topPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(topPenetrationDepth) < std::abs(bottomPenetrationDepth) &&
                (velocity.direction.y * velocity.ySpeed) < 0) {
                velocity.ySpeed = 0;
                transform.position.y = (wallCollider.y + wallCollider.h + positionOffset - yOffset);

                if (item->hasComponent<OnTopCollisionCallback>()) {
                    item->getComponent<OnTopCollisionCallback>().callback(item);
                }
                return;
            }

            // Bottom Side Collision
            if (std::abs(bottomPenetrationDepth) < std::abs(leftPenetrationDepth) &&
                std::abs(bottomPenetrationDepth) < std::abs(rightPenetrationDepth) &&
                std::abs(bottomPenetrationDepth) < std::abs(topPenetrationDepth) &&
                (velocity.direction.y * velocity.ySpeed) > 0) {
                velocity.ySpeed = 0;
                transform.position.y = wallCollider.y - itemCollider.h - positionOffset - yOffset;
                itemCollider.y = transform.position.y + yOffset;

                if (item->hasComponent<Gravity>()) {
                    item->getComponent<Gravity>().gravityEnabled = false;
                }

                if (item->hasComponent<OnBottomCollisionCallback>()) {
                    item->getComponent<OnBottomCollisionCallback>().callback(item);
                }
                return;
            }
    }
}