//
// Created by antho on 2026-03-23.
//

#ifndef TUTORIAL1_BEAKANIMATIONSYSTEM_H
#define TUTORIAL1_BEAKANIMATIONSYSTEM_H
#include <memory>
#include <string>

#include "Component.h"
#include "Entity.h"

class BeakAnimationSystem {
public:
    static std::string getAnimationClip(const std::unique_ptr<Entity>& beak) {
        auto& isFiring = beak->getComponent<IsFiring>();
        if (isFiring.startFiring) {
            return "opening_right";
        }
        if (isFiring.firing) {
            return "firing_right";
        }
        if (isFiring.endFiring) {
            return "closing_right";
        }
        return "closed_right";
    }
};

#endif //TUTORIAL1_BEAKANIMATIONSYSTEM_H