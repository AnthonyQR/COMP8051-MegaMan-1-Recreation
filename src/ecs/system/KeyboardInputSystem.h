//
// Created by antho on 2026-01-28.
//

#ifndef TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#define TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#include <iostream>
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Entity.h"

class KeyboardInputSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event event, float dt);
    };

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H