//
// Created by antho on 2026-04-29.
//

#ifndef MEGAMAN_ENEMYDEATHANIMATIONSYSTEM_H
#define MEGAMAN_ENEMYDEATHANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Entity.h"

class EnemyDeathAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& death) {
        NextAnimationClip newClip;
        newClip.name = "death";
        newClip.animationSpeed = 0.05f;
        newClip.destroyOnFinish = true;
        return newClip;
    }
};

#endif //MEGAMAN_ENEMYDEATHANIMATIONSYSTEM_H