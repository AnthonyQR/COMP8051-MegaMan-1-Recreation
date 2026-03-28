//
// Created by antho on 2026-03-27.
//

#ifndef TUTORIAL1_SPAWNONVISIBLESYSTEM_H
#define TUTORIAL1_SPAWNONVISIBLESYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "event/BaseEvent.h"

class SpawnOnVisibleSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities) {
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
            if (entity->hasComponent<SpawnOnVisible>() && entity->hasComponent<Transform>()) {
                auto& spawnOnVisible = entity->getComponent<SpawnOnVisible>();
                auto& transform = entity->getComponent<Transform>();

                // Check if entity is outside of the camera view
                if (
                    transform.position.x > cam.view.x + cam.view.w || // Right edge of view
                    transform.position.x < cam.view.x || // Left edge
                    transform.position.y > cam.view.y + cam.view.h || // Bottom edge
                    transform.position.y < cam.view.y // Top edge
                ) {
                    // Set it to not be visible
                    spawnOnVisible.isVisible = false;
                }

                // Run only once if spawner was not visible & now is
                else if (!spawnOnVisible.isVisible) {
                    spawnOnVisible.isVisible = true;
                    if (spawnOnVisible.spawnedEntity == nullptr) {
                        std::cout << "Spawn" << std::endl;
                        spawnOnVisible.spawnedEntity = spawnOnVisible.spawnCallback(transform);
                    }
                }
            }
        }
    }
};

#endif //TUTORIAL1_SPAWNONVISIBLESYSTEM_H