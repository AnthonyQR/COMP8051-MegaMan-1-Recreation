//
// Created by antho on 2026-04-04.
//

#ifndef TUTORIAL1_DEBUGTELEPORTSYSTEM_H
#define TUTORIAL1_DEBUGTELEPORTSYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Entity.h"

class World;

class DebugTeleportSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, SDL_Event event, World& world);
};
#endif //TUTORIAL1_DEBUGTELEPORTSYSTEM_H