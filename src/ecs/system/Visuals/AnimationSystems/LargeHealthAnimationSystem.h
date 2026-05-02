//
// Created by antho on 2026-05-01.
//

#ifndef MEGAMAN_LARGEHEALTHANIMATIONSYSTEM_H
#define MEGAMAN_LARGEHEALTHANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"

class Entity;

class LargeHealthAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& health) {
        NextAnimationClip newClip;
        newClip.name = "idle";
        newClip.animationSpeed = 0.2f;
        return newClip;
    }
};

#endif //MEGAMAN_LARGEHEALTHANIMATIONSYSTEM_H