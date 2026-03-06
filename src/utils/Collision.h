//
// Created by antho on 2026-02-04.
//

#ifndef TUTORIAL1_COLLISION_H
#define TUTORIAL1_COLLISION_H
#include <SDL3/SDL_rect.h>

#include "Component.h"

class Collision {
public:
    // Axis-Aligned Bounding Box
    static bool AABB(const SDL_FRect& rectA, const SDL_FRect& rectB);
    static bool AABB(const Collider& colA, const Collider& colB);

};

#endif //TUTORIAL1_COLLISION_H