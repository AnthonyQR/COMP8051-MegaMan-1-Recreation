//
// Created by antho on 2026-03-31.
//

#ifndef TUTORIAL1_MOVETOWARDSPLAYERSYSTEM_H
#define TUTORIAL1_MOVETOWARDSPLAYERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class MoveTowardsPlayerSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities) {
        Entity* player = nullptr;
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerTag>() &&
                entity->hasComponent<MainPlayer>()) {
                player = entity.get();
            }
        }
        if (player == nullptr) return;
        auto& playerTransform = player->getComponent<Transform>();

        for (auto& entity : entities) {
            if (entity->hasComponent<MoveTowardsPlayer>() &&
                entity->hasComponent<Velocity>() &&
                entity->hasComponent<Transform>()) {
                auto& moveTowardsPlayer = entity->getComponent<MoveTowardsPlayer>();
                auto& velocity = entity->getComponent<Velocity>();
                auto& transform = entity->getComponent<Transform>();

                if (!moveTowardsPlayer.isMovingTowards) continue;

                if (moveTowardsPlayer.moveX) {
                    if (playerTransform.position.x < transform.position.x) {
                        velocity.direction.x = -1;
                    }
                    else {
                        velocity.direction.x = 1;
                    }
                    velocity.xSpeed = moveTowardsPlayer.xSpeed;
                }

                if (moveTowardsPlayer.moveY) {
                    if (playerTransform.position.y < transform.position.y) {
                        velocity.direction.y = -1;
                    }
                    else {
                        velocity.direction.y = 1;
                    }
                    velocity.ySpeed = moveTowardsPlayer.ySpeed;
                }
            }
        }
    }
};

#endif //TUTORIAL1_MOVETOWARDSPLAYERSYSTEM_H