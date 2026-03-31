//
// Created by antho on 2026-03-31.
//

#ifndef TUTORIAL1_BLADERANIMATIONSYSTEM_H
#define TUTORIAL1_BLADERANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Component.h"
#include "Entity.h"

class BladerAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& blader) {
        NextAnimationClip newClip;
        auto& velocity = blader->getComponent<Velocity>();
        if (velocity.direction.x < 0) {
            newClip.name = "moving_left";
        }
        else {
            newClip.name = "moving_right";
        }
        newClip.animationSpeed = 0.15f;
        return newClip;
    }
};
#endif //TUTORIAL1_BLADERANIMATIONSYSTEM_H