//
// Created by antho on 2026-04-02.
//

#ifndef TUTORIAL1_OCTOPUSBATTERYSYSTEM_H
#define TUTORIAL1_OCTOPUSBATTERYSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class OctopusBatterySystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<OctopusBatteryStats>() &&
                entity->hasComponent<Velocity>()) {
                auto& stats = entity->getComponent<OctopusBatteryStats>();
                auto& velocity = entity->getComponent<Velocity>();

                if (velocity.xSpeed == 0 && velocity.ySpeed == 0) {
                    stats.timer -= dt;
                    if (stats.timer <= 0) {
                        stats.timer = stats.waitDuration;
                        if (velocity.direction.y != 0) {
                            velocity.direction.y = -velocity.direction.y;
                            velocity.ySpeed = stats.speed;
                        }
                        else {
                            velocity.direction.x = -velocity.direction.x;
                            velocity.xSpeed = stats.speed;
                        }
                    }
                }
            }
        }
    }
};

#endif //TUTORIAL1_OCTOPUSBATTERYSYSTEM_H