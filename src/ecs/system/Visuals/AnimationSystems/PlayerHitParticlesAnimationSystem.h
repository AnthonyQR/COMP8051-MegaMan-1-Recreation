//
// Created by antho on 2026-04-29.
//

#ifndef MEGAMAN_PLAYERHITPARTICLESANIMATIONSYSTEM_H
#define MEGAMAN_PLAYERHITPARTICLESANIMATIONSYSTEM_H
#include <memory>

#include "AnimationClip.h"
#include "Entity.h"

class PlayerHitParticlesAnimationSystem {
public:
    static NextAnimationClip getAnimationClip(const std::unique_ptr<Entity>& death) {
        NextAnimationClip newClip;
        newClip.name = "play";
        newClip.animationSpeed = 0.2f;
        newClip.destroyOnFinish = true;
        return newClip;
    }
};

#endif //MEGAMAN_PLAYERHITPARTICLESANIMATIONSYSTEM_H