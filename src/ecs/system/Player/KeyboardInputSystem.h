//
// Created by antho on 2026-01-28.
//

#ifndef TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#define TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#include <iostream>
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.h"
#include "Entity.h"

class KeyboardInputSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event event) {
        for (auto& e : entities) {
            if (e->hasComponent<KeyboardInputs>()) {
                auto& keyboardInputs = e->getComponent<KeyboardInputs>();

                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        // Jump
                        case SDLK_Z:
                            if (!keyboardInputs.isHoldingJump) {
                                keyboardInputs.isPressingJump = true;
                            }
                            keyboardInputs.isHoldingJump = true;
                            break;

                            // Attack
                        case SDLK_X:
                            keyboardInputs.isHoldingAttack = true;
                            break;

                            // Ladder Movement
                        case SDLK_UP:
                            keyboardInputs.isHoldingUp = true;
                            break;
                        case SDLK_DOWN:
                            keyboardInputs.isHoldingDown = true;
                            break;

                            // Horizontal Movement
                        case SDLK_LEFT:
                            keyboardInputs.isHoldingLeft = true;
                            break;
                        case SDLK_RIGHT:
                            keyboardInputs.isHoldingRight = true;
                            break;
                        default:
                            break;
                    }
                }

                if (event.type == SDL_EVENT_KEY_UP) {
                    switch (event.key.key) {
                        // Jump
                        case SDLK_Z:
                            keyboardInputs.isPressingJump = false;
                            keyboardInputs.isHoldingJump = false;
                            break;

                        case SDLK_X:
                            keyboardInputs.isHoldingAttack = false;
                            break;
                            // Ladder Movement
                        case SDLK_UP:
                            keyboardInputs.isHoldingUp = false;
                            break;
                        case SDLK_DOWN:
                            keyboardInputs.isHoldingDown = false;
                            break;
                            // Horizontal Movement
                        case SDLK_LEFT:
                            keyboardInputs.isHoldingLeft = false;
                            break;
                        case SDLK_RIGHT:
                            keyboardInputs.isHoldingRight = false;
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