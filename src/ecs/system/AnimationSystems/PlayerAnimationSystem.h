//
// Created by antho on 2026-03-17.
//

#ifndef TUTORIAL1_PLAYERANIMATIONSYSTEM_H
#define TUTORIAL1_PLAYERANIMATIONSYSTEM_H
#include <memory>
#include <string>

#include "Component.h"
#include "Entity.h"

class PlayerAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& player) {
        NextAnimationClip newClip;
        newClip = checkCurrentAction(player);
        newClip.name += checkIsFiring(player, newClip.name);
        return newClip;
    }

    static NextAnimationClip checkCurrentAction(const std::unique_ptr<Entity>& player) {
        NextAnimationClip newClip;
        float baseAnimationSpeed = player->getComponent<Animation>().speed;
        newClip.animationSpeed = baseAnimationSpeed;
        auto& velocity = player->getComponent<Velocity>();
        auto& isGrounded = player->getComponent<IsGrounded>().grounded;
        auto& isFacingRight = player->getComponent<IsFacingRight>().facingRight;
        auto& ladderClimbing = player->getComponent<LadderClimbing>();
        auto& hitKnockback = player->getComponent<HitKnockback>();

        if (hitKnockback.isHitKnockback) {
            if (hitKnockback.isRightHit) {
                newClip.name = "ground_hit_right";
            }
            else {
                newClip.name = "ground_hit_left";
            }
            return newClip;
        }

        if (ladderClimbing.isClimbing) {
            newClip.name = "climb";
            if (velocity.direction.y == 0) {
                newClip.animationSpeed = 0.0f;
            }
            else {
                newClip.animationSpeed = 0.16f;
            }
            return newClip;
        }

        if (isGrounded) {
            if (velocity.direction.x > 0) {
                newClip.name = "walk_right";
                return newClip;
            }
            if (velocity.direction.x < 0) {
                newClip.name = "walk_left";
                return newClip;
            }
            int random_int = rand() % 60;
            if (random_int == 1 && isFacingRight) {
                newClip.name = "idle_blink_right";
                return newClip;
            }
            if (random_int == 1 && !isFacingRight) {
                newClip.name = "idle_blink_right";
                return newClip;
            }

            if (isFacingRight) {
                newClip.name = "idle_right";
                return newClip;
            }
            newClip.name = "idle_left";
            return newClip;
        }

        if (!isGrounded) {
            if (isFacingRight) {
                newClip.name = "jump_right";
                return newClip;
            }
            newClip.name = "jump_left";
            return newClip;
        }
        return newClip;
    }

    static std::string checkIsFiring(const std::unique_ptr<Entity>& player, const std::string& clip) {
        auto& isFiring = player->getComponent<IsFiring>().firing;
        auto& isFacingRight = player->getComponent<IsFacingRight>().facingRight;
        auto& ladderClimbing = player->getComponent<LadderClimbing>();
        auto& hitKnockback = player->getComponent<HitKnockback>();

        if (hitKnockback.isHitKnockback) return "";
        if (!isFiring) return "";
        if (clip == "climb") {
            if (isFacingRight) {
                return "_right_fire";
            }
            else {
                return "_left_fire";
            }
        }
        return "_fire";
    }
};
#endif //TUTORIAL1_PLAYERANIMATIONSYSTEM_H