//
// Created by antho on 2026-03-19.
//

#ifndef TUTORIAL1_ISFIRINGTIMERSYSTEM_H
#define TUTORIAL1_ISFIRINGTIMERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class IsFiringTimerSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float dt) {
        for (auto& entity: entities) {
            if (entity->hasComponent<IsFiring>()) {
                auto& isFiring = entity->getComponent<IsFiring>();
                if (isFiring.startFiring) {
                    isFiring.timer -= dt;
                    if (isFiring.timer <= 0.0f) {
                        isFiring.startFiring = false;
                        isFiring.firing = true;
                        isFiring.timer = isFiring.firingDuration;
                    }
                }

                if (isFiring.firing) {
                    isFiring.timer -= dt;
                    if (isFiring.timer <= 0) {
                        isFiring.firing = false;
                        isFiring.endFiring = true;
                        isFiring.timer = isFiring.endFiringDuration;
                    }
                }

                if (isFiring.endFiring) {
                    isFiring.timer -= dt;
                    if (isFiring.timer <= 0.0f) {
                        isFiring.endFiring = false;
                    }
                }
            }
        }
    }
};
#endif //TUTORIAL1_ISFIRINGTIMERSYSTEM_H