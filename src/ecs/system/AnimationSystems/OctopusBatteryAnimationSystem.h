//
// Created by antho on 2026-04-02.
//

#ifndef TUTORIAL1_OCTOPUSBATTERYANIMATIONSYSTEM_H
#define TUTORIAL1_OCTOPUSBATTERYANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Entity.h"

class OctopusBatteryAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& octopus) {
        NextAnimationClip newClip;

        auto& velocity = octopus->getComponent<Velocity>();
        if (velocity.ySpeed != 0) {
            newClip.name = "start_move";
        }

        else {
            newClip.name = "end_move";
        }
        newClip.animationSpeed = 0.075f;
        newClip.stayOnLastFrame = true;
        return newClip;
    }
};

#endif //TUTORIAL1_OCTOPUSBATTERYANIMATIONSYSTEM_H