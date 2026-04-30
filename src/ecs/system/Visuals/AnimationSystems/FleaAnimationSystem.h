//
// Created by antho on 2026-04-28.
//

#ifndef MEGAMAN_FLEAANIMATIONSYSTEM_H
#define MEGAMAN_FLEAANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Component.h"
#include "Entity.h"

class FleaAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& blader) {
        NextAnimationClip newClip;
        auto& isGrounded = blader->getComponent<IsGrounded>();
        if (isGrounded.grounded) {
            newClip.name = "land";
        }
        else {
            newClip.name = "jump";
        }
        newClip.animationSpeed = 0.05f;
        newClip.stayOnLastFrame = true;
        return newClip;
    }
};

#endif //MEGAMAN_FLEAANIMATIONSYSTEM_H