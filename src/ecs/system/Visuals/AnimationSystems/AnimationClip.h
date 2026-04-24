//
// Created by antho on 2026-02-18.
//

#ifndef TUTORIAL1_ANIMATIONCLIP_H
#define TUTORIAL1_ANIMATIONCLIP_H
#include <vector>
#include <SDL3/SDL_rect.h>

struct AnimationClip {
    std::vector<SDL_FRect> frameIndices;
};

struct NextAnimationClip {
    std::string name{};
    float animationSpeed{};
    bool destroyOnFinish{};
    bool stayOnLastFrame{};
};

#endif //TUTORIAL1_ANIMATIONCLIP_H