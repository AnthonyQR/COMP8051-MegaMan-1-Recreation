//
// Created by antho on 2026-03-08.
//

#include "MainMenuSystem.h"

#include "Game.h"

void MainMenuSystem::update(std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event &event, World& world) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_SPACE: {
                for (auto& entity : entities) {
                    if (entity->hasComponent<GameStarting>()) {
                        auto& isStarting = entity->getComponent<GameStarting>().isStarting;
                        if (isStarting) return;

                        auto& transition(world.createEntity());
                        transition.addComponent<SceneTransitionDelay>(2.0f, "cutman");
                        isStarting = true;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
};
