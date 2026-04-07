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
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event event, float dt) {
        for (auto& e : entities) {
            if (e->hasComponent<PlayerTag>() &&
                e->hasComponent<Velocity>() &&
                e->hasComponent<PlayerHorizontalMovement>() &&
                e->hasComponent<IsGrounded>() &&
                e->hasComponent<Jump>() &&
                e->hasComponent<Gravity>() &&
                e->hasComponent<LadderClimbing>() &&
                e->hasComponent<Collider>() &&
                e->hasComponent<Transform>() &&
                e->hasComponent<KeyboardInputs>() &&
                e->hasComponent<IsFacingRight>() &&
                e->hasComponent<ProjectileStats>() &&
                e->hasComponent<ProjectileLimit>() &&
                e->hasComponent<IsFiring>() &&
                e->hasComponent<HasFired>() &&
                e->hasComponent<HitKnockback>()
                )
            {
                auto& v = e->getComponent<Velocity>();
                auto& movement = e->getComponent<PlayerHorizontalMovement>();
                auto& isGrounded = e->getComponent<IsGrounded>();
                auto& jump = e->getComponent<Jump>();
                auto& gravity = e->getComponent<Gravity>();
                auto& ladderClimbing = e->getComponent<LadderClimbing>();
                auto& collider = e->getComponent<Collider>();
                auto& transform = e->getComponent<Transform>();
                auto& keyboardInputs = e->getComponent<KeyboardInputs>();
                auto& isFacingRight = e->getComponent<IsFacingRight>();
                auto& projectileStats = e->getComponent<ProjectileStats>();
                auto& projectileLimit = e->getComponent<ProjectileLimit>();
                auto& isFiring = e->getComponent<IsFiring>();
                auto& hasFired = e->getComponent<HasFired>();
                auto& hitKnockback = e->getComponent<HitKnockback>();

                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        // Jump
                        case SDLK_Z:
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
                if (hitKnockback.isHitKnockback) return;

                // Jump
                if (keyboardInputs.isHoldingJump) {
                    if (!jump.hasJumped) {
                        jump.hasJumped = true;
                        if (ladderClimbing.isClimbing) {
                            ladderClimbing.isClimbing = false;
                            gravity.gravityEnabled = true;
                            v.ySpeed = 0;
                            v.direction.y = 1;
                        }
                        if (isGrounded.grounded) {
                            isGrounded.grounded = false;
                            jump.fastFalling = false;
                            gravity.gravityEnabled = true;
                            v.ySpeed = -jump.jumpSpeed;
                            v.direction.y = 1;
                        }
                    }
                }
                else {
                    if (v.ySpeed <= 0) {
                        jump.fastFalling = true;
                    }
                    jump.hasJumped = false;
                }

                if (keyboardInputs.isHoldingAttack) {
                    if (!hasFired.fired) {
                        hasFired.fired = true;
                        if (projectileLimit.currentProjectiles < projectileLimit.maxProjectiles) {
                            projectileLimit.currentProjectiles++;
                            isFiring.startFiring = true;
                            isFiring.timer = isFiring.startFiringDuration;
                            if (isFacingRight.facingRight) {
                                projectileStats.direction.x = 1;
                                projectileStats.spawnPoint.x = transform.position.x + collider.rect.w + collider.xOffset;
                            }
                            else {
                                projectileStats.direction.x = -1;
                                projectileStats.spawnPoint.x = transform.position.x;
                            }
                            if (!isGrounded.grounded) {
                                projectileStats.spawnPoint.y = transform.position.y + 24;
                            }
                            else {
                                projectileStats.spawnPoint.y = transform.position.y + 50;
                            }
                            projectileStats.spawnCallback(projectileStats);
                        }
                    }
                }

                else {
                    hasFired.fired = false;
                }

                // Ladder Climbing
                if (keyboardInputs.isHoldingUp && !keyboardInputs.isHoldingDown && ladderClimbing.canClimb) {
                    auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
                    if (ladderCollider.y < collider.rect.y) {
                        ladderClimbing.isClimbing = true;
                        v.direction.y = 1;
                        v.direction.x = 0;
                        v.ySpeed = -ladderClimbing.climbSpeed;
                        gravity.gravityEnabled = false;
                        transform.position.x = ladderCollider.x - collider.xOffset + 8.0f;
                    }
                }

                else if (keyboardInputs.isHoldingDown && !keyboardInputs.isHoldingUp && ladderClimbing.canClimb) {
                    auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
                    ladderClimbing.isClimbing = true;
                    v.direction.y = 1;
                    v.direction.x = 0;
                    v.ySpeed = ladderClimbing.climbSpeed;
                    gravity.gravityEnabled = false;
                    transform.position.x = ladderCollider.x - collider.xOffset + 8.0f;
                    if (ladderCollider.y > collider.rect.y) {
                        transform.position.y = ladderCollider.y - collider.yOffset;
                    }
                }

                else {
                    if (ladderClimbing.isClimbing) {
                        v.direction.y = 0;
                    }
                }

                if (ladderClimbing.isClimbing && isFiring.firing) {
                    v.direction.y = 0;
                }

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

                if (v.direction.x != 0) {
                    if (v.xSpeed == 0 && isGrounded.grounded) {
                        if (movement.isInching == false) {
                            movement.isInching = true;
                            transform.position.x += v.direction.x * movement.inchDistance;
                            movement.timer = movement.inchDuration;
                        }
                        else {
                            movement.timer -= dt;
                            if (movement.timer <= 0) {
                                v.xSpeed += movement.acceleration * dt;
                                movement.isInching = false;
                            }
                        }
                    }
                    else {
                        movement.isInching = false;
                        v.xSpeed += movement.acceleration * dt;
                        if (v.xSpeed > movement.maxSpeed) {
                            v.xSpeed = movement.maxSpeed;
                        }
                    }
                }
                else {
                    movement.isInching = false;
                    if (v.xSpeed != 0) {
                        if (isFacingRight.facingRight) {
                            v.direction.x = 1;
                        }
                        else {
                            v.direction.x = -1;
                        }
                    }
                    v.xSpeed -= movement.deacceleration * dt;
                    if (v.xSpeed < 0) {
                        v.xSpeed = 0;
                    }
                }




                if (ladderClimbing.isClimbing) {
                    v.direction.x = 0;
                }

            }
            }
        }
    };

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H