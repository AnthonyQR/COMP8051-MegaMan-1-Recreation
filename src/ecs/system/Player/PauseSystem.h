//
// Created by antho on 2026-04-07.
//

#ifndef TUTORIAL1_PAUSESYSTEM_H
#define TUTORIAL1_PAUSESYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Entity.h"

class World;

class PauseSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event& event, World& world);
};

#endif //TUTORIAL1_PAUSESYSTEM_H