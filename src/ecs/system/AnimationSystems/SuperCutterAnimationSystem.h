//
// Created by antho on 2026-04-03.
//

#ifndef TUTORIAL1_SUPERCUTTERANIMATIONSYSTEM_H
#define TUTORIAL1_SUPERCUTTERANIMATIONSYSTEM_H
#include "AnimationClip.h"
#include "Component.h"
#include "Entity.h"

class SuperCutterAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& superCutter) {
        NextAnimationClip newClip;
        auto& velocity = superCutter->getComponent<Velocity>();

        if (velocity.xSpeed <= 0) {
            newClip.name = "left";
        }
        else {
            newClip.name = "right";
        }
        newClip.animationSpeed = 0.075f;
        return newClip;
    }
};

#endif //TUTORIAL1_SUPERCUTTERANIMATIONSYSTEM_H