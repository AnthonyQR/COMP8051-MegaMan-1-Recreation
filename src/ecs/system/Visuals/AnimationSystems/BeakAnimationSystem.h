//
// Created by antho on 2026-03-23.
//

#ifndef TUTORIAL1_BEAKANIMATIONSYSTEM_H
#define TUTORIAL1_BEAKANIMATIONSYSTEM_H
#include <memory>
#include <string>

#include "AnimationClip.h"
#include "Component.h"
#include "Entity.h"

class BeakAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& beak) {
        NextAnimationClip newClip;
        newClip.name = checkCurrentAction(beak);
        newClip.name += checkFacingDirection(beak);
        newClip.animationSpeed = 0.08f;
        newClip.stayOnLastFrame = true;
        return newClip;
    }


    static std::string checkCurrentAction(const std::unique_ptr<Entity>& beak) {
        auto& isFiring = beak->getComponent<IsFiring>();
        if (isFiring.startFiring) {
            return "opening";
        }
        if (isFiring.firing) {
            return "firing";
        }
        if (isFiring.endFiring) {
            return "closing";
        }
        return "closed";
    }

    static std::string checkFacingDirection(const std::unique_ptr<Entity>& beak) {
        auto& isFacingRight = beak->getComponent<IsFacingRight>().facingRight;
        if (isFacingRight) {
            return "_left";
        }
        return "_right";
    }
};

#endif //TUTORIAL1_BEAKANIMATIONSYSTEM_H