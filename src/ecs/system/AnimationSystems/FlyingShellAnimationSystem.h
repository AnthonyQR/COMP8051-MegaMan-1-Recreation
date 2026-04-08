//
// Created by antho on 2026-04-07.
//

#ifndef TUTORIAL1_FLYINGSHELLANIMATIONSYSTEM_H
#define TUTORIAL1_FLYINGSHELLANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Component.h"
#include "Entity.h"

class FlyingShellAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& shell) {
        NextAnimationClip newClip;
        auto& velocity = shell->getComponent<Velocity>();
        auto& facingRight = shell->getComponent<IsFacingRight>().facingRight;
        if (velocity.direction.x != 0) {
            newClip.name = "closed";
        }
        else if (!facingRight) {
            newClip.name = "fire_left";
        }
        else {
            newClip.name = "fire_right";
        }
        newClip.animationSpeed = 0.0f;
        return newClip;
    }
};

#endif //TUTORIAL1_FLYINGSHELLANIMATIONSYSTEM_H