//
// Created by antho on 2026-04-07.
//

#include "PauseSystem.h"

#include "Game.h"

void PauseSystem::update(std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event &event, World& world) {
    for (auto& entity : entities) {
        if (entity->hasComponent<GamePaused>()) {
            auto& paused = entity->getComponent<GamePaused>().hasPaused;
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_RETURN: {
                        if (!paused) {
                            paused = true;
                            Game::gameState.isPaused = !Game::gameState.isPaused;
                            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("pause"));
                            std::cout << Game::gameState.isPaused << std::endl;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            if (event.type == SDL_EVENT_KEY_UP) {
                switch (event.key.key) {
                    case SDLK_RETURN: {
                        paused = false;
                        break;
                    }
                    default:
                        break;
                }
            }
            return;
        }
    }

};
