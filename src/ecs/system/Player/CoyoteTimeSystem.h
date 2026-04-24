//
// Created by antho on 2026-03-29.
//

#ifndef TUTORIAL1_COYOTETIMESYSTEM_H
#define TUTORIAL1_COYOTETIMESYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class CoyoteTimeSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<CoyoteTime>() &&
                entity->hasComponent<Gravity>() &&
                entity->hasComponent<IsGrounded>()) {
                auto& coyoteTime = entity->getComponent<CoyoteTime>();
                auto& gravity = entity->getComponent<Gravity>();
                auto& isGrounded = entity->getComponent<IsGrounded>();


                if (coyoteTime.isCoyoteTime) {
                    coyoteTime.timer -= dt;
                    if (coyoteTime.timer <= 0) {
                        gravity.gravityEnabled = true;
                        isGrounded.grounded = false;
                    }
                }
            }
        }
    }
};

#endif //TUTORIAL1_COYOTETIMESYSTEM_H