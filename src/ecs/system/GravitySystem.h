//
// Created by antho on 2026-03-10.
//

#ifndef TUTORIAL1_GRAVITYSYSTEM_H
#define TUTORIAL1_GRAVITYSYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.h"
#include "Entity.h"

class GravitySystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& e : entities) {
            if (e->hasComponent<Velocity>() && e->hasComponent<Gravity>()) {
                auto& velocity = e->getComponent<Velocity>();
                auto& gravity = e->getComponent<Gravity>();
                if (gravity.gravityEnabled == true) {
                    if (e->hasComponent<Jump>()) {
                        auto& jump = e->getComponent<Jump>();
                        if (jump.fastFalling && velocity.ySpeed <= 0) {
                            velocity.ySpeed += jump.fastFallSpeed * dt;
                            continue;
                        }
                        else if (velocity.ySpeed >= 0) {
                            jump.fastFalling = false;
                        }
                    }

                    velocity.ySpeed += gravity.gravitySpeed * dt;
                    velocity.direction.y = 1;
                    if (velocity.ySpeed >= gravity.fallingCap) {
                        velocity.ySpeed = gravity.fallingCap;
                    }
                }
            }
        };
    };
};

#endif //TUTORIAL1_GRAVITYSYSTEM_H