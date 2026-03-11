//
// Created by antho on 2026-01-28.
//

#ifndef TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#define TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.h"
#include "Entity.h"

class KeyboardInputSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event event) {
        for (auto& e : entities) {
            if (e->hasComponent<PlayerTag>() && e->hasComponent<Velocity>()) {
                auto& v = e->getComponent<Velocity>();
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        case SDLK_SPACE:
                            if (e->hasComponent<IsGrounded>() && e->hasComponent<Gravity>()) {
                                auto& isGrounded = e->getComponent<IsGrounded>();
                                if (isGrounded.grounded) {
                                    isGrounded.grounded = false;
                                    e->getComponent<Gravity>().gravityEnabled = true;
                                    v.direction.y = -4;
                                }
                            }
                            break;
                        case SDLK_A:
                            v.direction.x = -1;
                            break;
                        case SDLK_D:
                            v.direction.x = 1;
                            break;
                        default:
                            break;
                    }
                }

                if (event.type == SDL_EVENT_KEY_UP) {
                    switch (event.key.key) {
                        case SDLK_SPACE:
                            if (v.direction.y <= 0) {
                                v.direction.y = 0;
                            }
                            break;
                        case SDLK_A:
                            v.direction.x = 0;
                            break;
                        case SDLK_D:
                            v.direction.x = 0;
                            break;
                        default:
                            break;
                    }
                }
                }
            }
        }
    };

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H