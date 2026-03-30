//
// Created by antho on 2026-03-29.
//

#ifndef TUTORIAL1_RENDERUTILS_H
#define TUTORIAL1_RENDERUTILS_H
#include <SDL3/SDL_rect.h>

class RenderUtils {
public:
    static SDL_FRect getScaledDest(const SDL_FRect& dest, float scale) {
        return SDL_FRect {
            dest.x + dest.w * (1 - scale) / 2,
            dest.y + dest.h * (1 - scale) / 2,
            dest.w * scale,
            dest.h * scale
        };
    }
};

#endif //TUTORIAL1_RENDERUTILS_H