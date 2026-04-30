//
// Created by antho on 2026-04-27.
//

#ifndef MEGAMAN_AUTOJUMPSYSTEM_H
#define MEGAMAN_AUTOJUMPSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class AutoJumpSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<AutoJump>() &&
                entity->hasComponent<Jump>() &&
                entity->hasComponent<Velocity>() &&
                entity->hasComponent<Gravity>()) {

                auto& autoJump = entity->getComponent<AutoJump>();
                auto& jump = entity->getComponent<Jump>();
                auto& velocity = entity->getComponent<Velocity>();

                if (!autoJump.prepareJump) continue;

                autoJump.jumpDelayTimer -= dt;
                if (autoJump.jumpDelayTimer < 0) {
                    autoJump.prepareJump = false;
                    jump.startJumping = true;

                    // Pick a random jump pattern from the list
                    Vector2D jumpSpeed = autoJump.jumpPatterns[rand() % autoJump.jumpPatterns.size()];
                    jump.jumpSpeed = jumpSpeed.y;
                    velocity.xSpeed = jumpSpeed.x;
                    velocity.direction.x = 1;
                }
            }
        }
    }
};

#endif //MEGAMAN_AUTOJUMPSYSTEM_H