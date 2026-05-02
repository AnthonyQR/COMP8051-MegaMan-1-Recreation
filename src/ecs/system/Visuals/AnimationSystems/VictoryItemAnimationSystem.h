//
// Created by antho on 2026-04-29.
//

#ifndef MEGAMAN_VICTORYITEMANIMATIONSYSTEM_H
#define MEGAMAN_VICTORYITEMANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Entity.h"

class VictoryItemAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& item) {
        NextAnimationClip newClip;
        newClip.name = "play";
        newClip.animationSpeed = 0.2f;
        return newClip;
    }
};


#endif //MEGAMAN_VICTORYITEMANIMATIONSYSTEM_H