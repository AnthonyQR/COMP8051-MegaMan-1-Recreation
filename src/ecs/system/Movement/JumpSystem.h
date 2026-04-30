//
// Created by antho on 2026-04-27.
//

#ifndef MEGAMAN_JUMPSYSTEM_H
#define MEGAMAN_JUMPSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class JumpSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Jump>() &&
                entity->hasComponent<Velocity>() &&
                entity->hasComponent<Gravity>()) {
                auto& jump = entity->getComponent<Jump>();
                auto& velocity = entity->getComponent<Velocity>();
                auto& gravity = entity->getComponent<Gravity>();

                if (!jump.startJumping) continue;
                jump.startJumping = false;

                // Mainly for player to get off ladder when pressing jump while climbing
                if (entity->hasComponent<LadderClimbing>()) {
                    auto& ladderClimbing = entity->getComponent<LadderClimbing>();
                    if (ladderClimbing.isClimbing) {
                        ladderClimbing.isClimbing = false;
                        gravity.gravityEnabled = true;
                        velocity.ySpeed = 0;
                        velocity.direction.y = 1;
                        continue;
                    }
                }

                // Jump Logic
                jump.fastFalling = false;
                gravity.gravityEnabled = true;
                velocity.ySpeed = -jump.jumpSpeed;
                velocity.direction.y = 1;
                if (entity->hasComponent<IsGrounded>()) {
                    entity->getComponent<IsGrounded>().grounded = false;
                }

                // For entity specific jump logic (ex. Hurtbox Shifting)
                if (entity->hasComponent<OnJumpCallback>()) {
                    entity->getComponent<OnJumpCallback>().callback(entity.get());
                }
            }
        }
    };
};

#endif //MEGAMAN_JUMPSYSTEM_H