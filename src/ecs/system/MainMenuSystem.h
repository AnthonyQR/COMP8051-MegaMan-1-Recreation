//
// Created by antho on 2026-03-08.
//

#ifndef TUTORIAL1_MAINMENUSYSTEM_H
#define TUTORIAL1_MAINMENUSYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Entity.h"

class World;

class MainMenuSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event& event, World& world);
};

#endif //TUTORIAL1_MAINMENUSYSTEM_H