//
// Created by antho on 2026-04-07.
//

#ifndef TUTORIAL1_SPAWNWHILEVISIBLETIMERSYSTEM_H
#define TUTORIAL1_SPAWNWHILEVISIBLETIMERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class SpawnWhileVisibleSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        Entity* cameraEntity = nullptr;

        // Find camera
        for (auto& e : entities) {
            if (e->hasComponent<Camera>()) {
                cameraEntity = e.get();
                break;
            }
        }

        if (!cameraEntity) return;
        auto& cam = cameraEntity->getComponent<Camera>();

        for (auto& entity : entities) {
            if (entity->hasComponent<SpawnWhileVisibleTimer>() && entity->hasComponent<Transform>()) {
                auto& spawnWhileVisible = entity->getComponent<SpawnWhileVisibleTimer>();
                auto& transform = entity->getComponent<Transform>();

                // Check if entity is outside of the camera view
                if (
                    transform.position.x > cam.view.x + cam.view.w || // Right edge of view
                    transform.position.x < cam.view.x || // Left edge
                    transform.position.y > cam.view.y + cam.view.h || // Bottom edge
                    transform.position.y < cam.view.y // Top edge
                ) {
                    spawnWhileVisible.timer = 0;
                    continue;
                }

                if (spawnWhileVisible.spawnedEntity == nullptr && spawnWhileVisible.timer <= 0) {
                    spawnWhileVisible.spawnedEntity = spawnWhileVisible.spawnCallback(transform);
                    spawnWhileVisible.timer = spawnWhileVisible.longDelay;
                }
                else {
                    spawnWhileVisible.timer -= dt;
                }
            }
        }
    }
};
#endif //TUTORIAL1_SPAWNWHILEVISIBLETIMERSYSTEM_H