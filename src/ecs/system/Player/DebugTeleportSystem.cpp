//
// Created by antho on 2026-04-04.
//

#include "DebugTeleportSystem.h"

#include "Component.h"
#include "World.h"

void DebugTeleportSystem::update(const std::vector<std::unique_ptr<Entity>> &entities,
                                 const SDL_Event event, World& world) {

    for (auto& e : entities) {
        if (e->hasComponent<PlayerTag>() &&
            !e->hasComponent<PlayerGroundCheck>() &&
            !e->hasComponent<ProjectileTag>() &&
            !e->hasComponent<PlayerHurtbox>()) {
            auto& transform = e->getComponent<Transform>();
            auto& xOffset = e->getComponent<Collider>().xOffset;
            auto& spawnPoints = world.getMap().debugSpawnPoints;
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_1:
                        if (0 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(0);
                            transform.position.x -= xOffset;
                        }

                        break;
                    case SDLK_2:
                        if (1 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(1);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_3:
                        if (2 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(2);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_4:
                        if (3 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(3);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_5:
                        if (4 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(4);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_6:
                        if (5 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(5);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_7:
                        if (6 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(6);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_8:
                        if (7 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(7);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_9:
                        if (8 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(8);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_0:
                        if (9 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(9);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_MINUS:
                        if (10 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(10);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_EQUALS:
                        if (11 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(11);
                            transform.position.x -= xOffset;
                        }
                        break;
                    case SDLK_BACKSPACE:
                        if (12 < spawnPoints.size()) {
                            transform.position = spawnPoints.at(12);
                            transform.position.x -= xOffset;
                        }
                        break;
                }
            }
        }
    }
}


