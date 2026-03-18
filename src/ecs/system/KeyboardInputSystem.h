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
            if (e->hasComponent<PlayerTag>() &&
                e->hasComponent<Velocity>() &&
                e->hasComponent<IsGrounded>() &&
                e->hasComponent<Jump>() &&
                e->hasComponent<Gravity>() &&
                e->hasComponent<LadderClimbing>() &&
                e->hasComponent<Collider>() &&
                e->hasComponent<Transform>() &&
                e->hasComponent<KeyboardInputs>() &&
                e->hasComponent<IsFacingRight>()
                )
            {
                auto& v = e->getComponent<Velocity>();
                auto& isGrounded = e->getComponent<IsGrounded>();
                auto& jump = e->getComponent<Jump>();
                auto& gravity = e->getComponent<Gravity>();
                auto& ladderClimbing = e->getComponent<LadderClimbing>();
                auto& collider = e->getComponent<Collider>();
                auto& transform = e->getComponent<Transform>();
                auto& keyboardInputs = e->getComponent<KeyboardInputs>();
                auto& isFacingRight = e->getComponent<IsFacingRight>();

                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        // Jump
                        case SDLK_SPACE:
                            keyboardInputs.isHoldingJump = true;
                            break;

                        // Ladder Movement
                        case SDLK_W:
                            keyboardInputs.isHoldingUp = true;
                            break;
                        case SDLK_S:
                            keyboardInputs.isHoldingDown = true;
                            break;

                        // Horizontal Movement
                        case SDLK_A:
                            keyboardInputs.isHoldingLeft = true;
                            break;
                        case SDLK_D:
                            keyboardInputs.isHoldingRight = true;
                            break;
                        default:
                            break;
                    }
                }

                if (event.type == SDL_EVENT_KEY_UP) {
                    switch (event.key.key) {
                        // Jump
                        case SDLK_SPACE:
                            keyboardInputs.isHoldingJump = false;
                            break;

                        // Ladder Movement
                        case SDLK_W:
                            keyboardInputs.isHoldingUp = false;
                            break;
                        case SDLK_S:
                            keyboardInputs.isHoldingDown = false;
                            break;
                        // Horizontal Movement
                        case SDLK_A:
                            keyboardInputs.isHoldingLeft = false;
                            break;
                        case SDLK_D:
                            keyboardInputs.isHoldingRight = false;
                            break;
                        default:
                            break;
                    }
                }

                // Jump
                if (keyboardInputs.isHoldingJump) {
                    if (ladderClimbing.isClimbing) {
                        ladderClimbing.isClimbing = false;
                        gravity.gravityEnabled = true;
                        v.ySpeed = 0;
                        v.direction.y = 1;
                    }
                    if (isGrounded.grounded && !jump.hasJumped) {
                        isGrounded.grounded = false;
                        jump.hasJumped = true;
                        jump.fastFalling = false;
                        gravity.gravityEnabled = true;
                        v.ySpeed = -jump.jumpSpeed;
                        v.direction.y = 1;
                    }
                }
                else {
                    if (v.ySpeed <= 0) {
                        jump.fastFalling = true;
                    }
                    jump.hasJumped = false;
                }

                // Ladder Climbing
                if (keyboardInputs.isHoldingUp && !keyboardInputs.isHoldingDown && ladderClimbing.canClimb) {
                    auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
                    if (ladderCollider.y < collider.rect.y) {
                        std::cout << "up" << std::endl;
                        ladderClimbing.isClimbing = true;
                        v.direction.y = 1;
                        v.direction.x = 0;
                        v.ySpeed = -ladderClimbing.climbSpeed;
                        gravity.gravityEnabled = false;
                        transform.position.x = ladderCollider.x - collider.xOffset - 6;
                    }
                }

                else if (keyboardInputs.isHoldingDown && !keyboardInputs.isHoldingUp && ladderClimbing.canClimb) {
                    auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
                    ladderClimbing.isClimbing = true;
                    v.direction.y = 1;
                    v.direction.x = 0;
                    v.ySpeed = ladderClimbing.climbSpeed;
                    gravity.gravityEnabled = false;
                    transform.position.x = ladderCollider.x - collider.xOffset - 6;
                    if (ladderCollider.y > collider.rect.y) {
                        transform.position.y = ladderCollider.y + 3;
                    }
                }

                else {
                    if (ladderClimbing.isClimbing) {
                        v.direction.y = 0;
                    }
                }

                if (!ladderClimbing.isClimbing) {
                    if (keyboardInputs.isHoldingLeft && !keyboardInputs.isHoldingRight) {
                        v.direction.x = -1;
                        isFacingRight.facingRight = false;
                    }
                    else if (keyboardInputs.isHoldingRight && !keyboardInputs.isHoldingLeft) {
                        v.direction.x = 1;
                        isFacingRight.facingRight = true;
                    }
                    else {
                        v.direction.x = 0;
                    }
                }
                else {
                    v.direction.x = 0;
                }
            }
            }
        }
    };

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H