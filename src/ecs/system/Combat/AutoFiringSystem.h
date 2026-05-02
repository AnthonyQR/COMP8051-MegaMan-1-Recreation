//
// Created by antho on 2026-03-23.
//

#ifndef TUTORIAL1_AUTOFIRINGSYSTEM_H
#define TUTORIAL1_AUTOFIRINGSYSTEM_H
#include <iostream>
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class AutoFiringSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float dt) {
        for (auto& entity: entities) {
            if (entity->hasComponent<IsFiring>() && entity->hasComponent<AutoFiring>() && entity->hasComponent<ProjectileStats>()) {
                auto& isFiring = entity->getComponent<IsFiring>();
                auto& autoFiring = entity->getComponent<AutoFiring>();
                auto& projectileStats = entity->getComponent<ProjectileStats>();
                if (isFiring.firing) {
                    autoFiring.timer -= dt;
                    if (autoFiring.timer <= 0) {
                        if (autoFiring.nextPattern >= autoFiring.patterns.size()) {
                            continue;
                        }
                        if (autoFiring.burst) {
                            while (autoFiring.nextPattern < autoFiring.patterns.size()) {
                                projectileStats.direction = autoFiring.patterns.at(autoFiring.nextPattern).direction;
                                projectileStats.spawnCallback(projectileStats, *entity.get());

                                autoFiring.nextPattern++;
                            }
                        }
                        else {
                            projectileStats.direction = autoFiring.patterns.at(autoFiring.nextPattern).direction;
                            projectileStats.spawnCallback(projectileStats, *entity.get());

                            autoFiring.nextPattern++;
                            if (autoFiring.nextPattern < autoFiring.patterns.size()) {
                                autoFiring.timer = autoFiring.patterns.at(autoFiring.nextPattern).interval;
                            }
                        }
                    }
                }

                else if (isFiring.endFiring) {
                    autoFiring.timer = autoFiring.firingDelay;
                }

                else if (!isFiring.startFiring && !isFiring.firing && !isFiring.endFiring) {
                    autoFiring.nextPattern = 0;
                    if (autoFiring.loop) {
                        autoFiring.timer -= dt;
                        if (autoFiring.timer <= 0) {
                            isFiring.startFiring = true;
                            isFiring.timer = isFiring.startFiringDuration;
                            autoFiring.timer = autoFiring.patterns.at(autoFiring.nextPattern).interval;

                            if (entity->hasComponent<OnStartFiringCallback>()) {
                                entity->getComponent<OnStartFiringCallback>().callback(entity.get());
                            }
                        }
                    }
                }
            }
        }
    }
};


#endif //TUTORIAL1_AUTOFIRINGSYSTEM_H