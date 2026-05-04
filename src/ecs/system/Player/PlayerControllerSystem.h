//
// Created by antho on 2026-05-04.
//

#ifndef MEGAMAN_PLAYERCONTROLLERSYSTEM_H
#define MEGAMAN_PLAYERCONTROLLERSYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Entity.h"

class PlayerControllerSystem {
    public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& e : entities) {
            if (e->hasComponent<PlayerTag>() && e->hasComponent<MainPlayer>()) {
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
                auto& sprite = e->getComponent<Sprite>();

                if (hitKnockback.isHitKnockback) return;

                // Jump
                if (keyboardInputs.isPressingJump) {
                    keyboardInputs.isPressingJump = false;
                    if (isGrounded.grounded || ladderClimbing.isClimbing) {
                        jump.startJumping = true;
                    }
                }

                else if (!keyboardInputs.isHoldingJump){
                    if (v.ySpeed <= 0) {
                        jump.fastFalling = true;
                    }
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
                                projectileStats.spawnPoint.y = transform.position.y + 51;
                            }
                            projectileStats.spawnCallback(projectileStats, *e.get());
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

                        transform.position.x = ladderCollider.x - (sprite.dst.w / 4);
                    }
                }

                else if (keyboardInputs.isHoldingDown && !keyboardInputs.isHoldingUp && ladderClimbing.canClimb) {
                    auto& ladderCollider = ladderClimbing.ladderEntity->getComponent<Collider>().rect;
                    ladderClimbing.isClimbing = true;
                    v.direction.y = 1;
                    v.direction.x = 0;
                    v.ySpeed = ladderClimbing.climbSpeed;
                    gravity.gravityEnabled = false;
                    transform.position.x = ladderCollider.x - (sprite.dst.w / 4);
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
                    if (v.direction.x == 1) v.xSpeed = 0;
                    v.direction.x = -1;
                    isFacingRight.facingRight = false;
                }
                else if (keyboardInputs.isHoldingRight && !keyboardInputs.isHoldingLeft) {
                    if (v.direction.x == -1) v.xSpeed = 0;
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



#endif //MEGAMAN_PLAYERCONTROLLERSYSTEM_H