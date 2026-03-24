//
// Created by antho on 2026-03-23.
//

#ifndef TUTORIAL1_AUTOFIRINGSYSTEM_H
#define TUTORIAL1_AUTOFIRINGSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class AutoFiringSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float dt) {
        for (auto& entity: entities) {
            if (entity->hasComponent<IsFiring>() && entity->hasComponent<AutoFiring>() && entity->hasComponent<ProjectileStats>()) {
                auto& isFiring = entity->getComponent<IsFiring>().firing;
                auto& autoFiring = entity->getComponent<AutoFiring>();
                auto& projectileStats = entity->getComponent<ProjectileStats>();
                if (isFiring) {
                    autoFiring.timer -= dt;
                    if (autoFiring.timer <= 0) {
                        if (autoFiring.oneShot && (autoFiring.nextPattern + 1 > autoFiring.patterns.size())) {
                            return;
                        }
                        projectileStats.direction = autoFiring.patterns.at(autoFiring.nextPattern).direction;
                        projectileStats.spawnCallback(projectileStats);

                        autoFiring.nextPattern = (autoFiring.nextPattern + 1) % autoFiring.patterns.size();
                        autoFiring.timer = autoFiring.patterns.at(autoFiring.nextPattern).interval;
                    }
                }
                else {
                    autoFiring.nextPattern = 0;
                    autoFiring.timer = autoFiring.patterns.at(autoFiring.nextPattern).interval;
                }
            }
        }
    }
};


#endif //TUTORIAL1_AUTOFIRINGSYSTEM_H