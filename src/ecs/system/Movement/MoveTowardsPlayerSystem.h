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
        for (auto& entity : entities) {
            if (entity->hasComponent<MoveTowardsPlayer>() &&
                entity->hasComponent<TrackPlayer>() &&
                entity->hasComponent<Velocity>()) {
                auto& moveTowardsPlayer = entity->getComponent<MoveTowardsPlayer>();
                auto& trackPlayer = entity->getComponent<TrackPlayer>();
                auto& velocity = entity->getComponent<Velocity>();

                if (!moveTowardsPlayer.isMovingTowards) continue;

                if (moveTowardsPlayer.moveX) {
                    if (trackPlayer.isPlayerToTheRight) {
                        velocity.direction.x = 1;
                    }
                    else {
                        velocity.direction.x = -1;
                    }
                    velocity.xSpeed = moveTowardsPlayer.xSpeed;
                }

                if (moveTowardsPlayer.moveY) {
                    if (trackPlayer.isPlayerBelow) {
                        velocity.direction.y = 1;
                    }
                    else {
                        velocity.direction.y = -1;
                    }
                    velocity.ySpeed = moveTowardsPlayer.ySpeed;
                }
            }
        }
    }
};

#endif //TUTORIAL1_MOVETOWARDSPLAYERSYSTEM_H