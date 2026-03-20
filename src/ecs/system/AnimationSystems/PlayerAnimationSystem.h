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
    static std::string getAnimationClip(const std::unique_ptr<Entity>& player) {
        std::string clip = checkCurrentAction(player);
        clip += checkIsFiring(player, clip);
        return clip;
    }

    static std::string checkCurrentAction(const std::unique_ptr<Entity>& player) {
        auto& velocity = player->getComponent<Velocity>();
        auto& isGrounded = player->getComponent<IsGrounded>().grounded;
        auto& isFacingRight = player->getComponent<IsFacingRight>().facingRight;
        auto& ladderClimbing = player->getComponent<LadderClimbing>();


        if (ladderClimbing.isClimbing) {
            return "climb";
        }

        if (isGrounded) {
            if (velocity.direction.x > 0) {
                return "walk_right";
            }
            if (velocity.direction.x < 0) {
                return "walk_left";
            }
            int random_int = rand() % 60;
            if (random_int == 1 && isFacingRight) {
                return "idle_blink_right";
            }
            if (random_int == 1 && !isFacingRight) {
                return "idle_blink_left";
            }

            if (isFacingRight) {
                return "idle_right";
            }
            return "idle_left";
        }

        if (!isGrounded) {
            if (isFacingRight) {
                return "jump_right";
            }
            return "jump_left";
        }
        return "";
    }

    static std::string checkIsFiring(const std::unique_ptr<Entity>& player, const std::string& clip) {
        auto& isFiring = player->getComponent<IsFiring>().firing;
        auto& isFacingRight = player->getComponent<IsFacingRight>().facingRight;
        auto& ladderClimbing = player->getComponent<LadderClimbing>();

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