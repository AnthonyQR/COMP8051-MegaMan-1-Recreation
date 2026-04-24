//
// Created by antho on 2026-03-24.
//

#ifndef TUTORIAL1_TITLESCREENANIMATIONSYSTEM_H
#define TUTORIAL1_TITLESCREENANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Component.h"
#include "Entity.h"

class TitleScreenAnimationSystem {
public:
    static NextAnimationClip getAnimationClip (const std::unique_ptr<Entity>& titleScreen) {
        NextAnimationClip newClip;
        newClip.animationSpeed = 0.25f;
        auto& gameStarting = titleScreen->getComponent<GameStarting>();

        if (gameStarting.isStarting) {
            newClip.name = "starting";
        }
        else {
            newClip.name = "idle";
        }

        return newClip;
    }
};

#endif //TUTORIAL1_TITLESCREENANIMATIONSYSTEM_H