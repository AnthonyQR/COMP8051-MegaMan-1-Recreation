//
// Created by antho on 2026-04-07.
//

#ifndef TUTORIAL1_STOPMOVEMENTWHILEFIRINGSYSTEM_H
#define TUTORIAL1_STOPMOVEMENTWHILEFIRINGSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class StopMovementWhileFiringSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<StopMovementWhileFiring>() &&
                entity->hasComponent<IsFiring>() &&
                entity->hasComponent<Velocity>()) {
                auto& movement = entity->getComponent<StopMovementWhileFiring>();
                auto& isFiring = entity->getComponent<IsFiring>();
                auto& velocity = entity->getComponent<Velocity>();

                if (isFiring.startFiring || isFiring.firing || isFiring.endFiring) {
                    velocity.direction = Vector2D(0, 0);
                }
                else {
                    velocity.direction = movement.direction;
                }
            }
        }
    }
};

#endif //TUTORIAL1_STOPMOVEMENTWHILEFIRINGSYSTEM_H