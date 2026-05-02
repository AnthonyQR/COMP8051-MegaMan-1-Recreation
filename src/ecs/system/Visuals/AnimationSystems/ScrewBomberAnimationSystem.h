//
// Created by antho on 2026-05-01.
//

#ifndef MEGAMAN_SCREWBOMBERANIMATIONSYSTEM_H
#define MEGAMAN_SCREWBOMBERANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Component.h"
#include "Entity.h"

class ScrewBomberAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& screwBomber) {
        NextAnimationClip newClip;
        newClip.name = checkCurrentAction(screwBomber);
        if (newClip.name != "shooting") {
            newClip.stayOnLastFrame = true;
        }
        newClip.name += checkFacingDirection(screwBomber);
        newClip.animationSpeed = 0.065f;
        return newClip;
    }


    static std::string checkCurrentAction(const std::unique_ptr<Entity>& screwBomber) {
        auto& isFiring = screwBomber->getComponent<IsFiring>();
        if (isFiring.startFiring) {
            return "start_shooting";

        }
        if (isFiring.firing) {
            return "shooting";
        }
        if (isFiring.endFiring) {
            return "stop_shooting";
        }
        return "stop_shooting";
    }

    static std::string checkFacingDirection(const std::unique_ptr<Entity>& screwBomber) {
        auto& autoFiringPatterns = screwBomber->getComponent<AutoFiring>().patterns;
        if (autoFiringPatterns[1].direction.y > 0) {
            return "_top";
        }
        return "_bottom";
    }
};

#endif //MEGAMAN_SCREWBOMBERANIMATIONSYSTEM_H