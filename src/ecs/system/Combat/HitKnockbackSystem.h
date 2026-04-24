//
// Created by antho on 2026-03-25.
//

#ifndef TUTORIAL1_HITKNOCKBACKSYSTEM_H
#define TUTORIAL1_HITKNOCKBACKSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class HitKnockbackSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<HitKnockback>() &&
                entity->hasComponent<Velocity>()) {
                auto& hitKnockback = entity->getComponent<HitKnockback>();
                auto& velocity = entity->getComponent<Velocity>();

                if (hitKnockback.isHitKnockback) {
                    if (hitKnockback.isRightHit) {
                        velocity.direction.x = -1;
                    }
                    else {
                        velocity.direction.x = 1;
                    }
                    velocity.xSpeed = hitKnockback.knockbackSpeed;

                    hitKnockback.timer -= dt;
                    if (hitKnockback.timer <= 0) {
                        hitKnockback.isHitKnockback = false;
                    }
                }
            }
        }
    }
};

#endif //TUTORIAL1_HITKNOCKBACKSYSTEM_H