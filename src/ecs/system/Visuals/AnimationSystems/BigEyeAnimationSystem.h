//
// Created by antho on 2026-04-30.
//

#ifndef MEGAMAN_BIGEYEANIMATIONSYSTEM_H
#define MEGAMAN_BIGEYEANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Component.h"
#include "Entity.h"

class BigEyeAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& bigEye) {
        NextAnimationClip newClip;
        auto& isGrounded = bigEye->getComponent<IsGrounded>();
        auto& isFacingRight = bigEye->getComponent<IsFacingRight>().facingRight;
        if (isGrounded.grounded) {
            newClip.name = "land";
        }
        else {
            newClip.name = "jump";
        }

        if (isFacingRight) {
            newClip.name += "_right";
        }
        else {
            newClip.name += "_left";
        }
        newClip.animationSpeed = 0.05f;
        newClip.stayOnLastFrame = true;
        return newClip;
    }
};

#endif //MEGAMAN_BIGEYEANIMATIONSYSTEM_H