//
// Created by antho on 2026-02-25.
//

#ifndef TUTORIAL1_DESTRUCTIONSYSTEM_H
#define TUTORIAL1_DESTRUCTIONSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class DestructionSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {
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

        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e -> hasComponent<ProjectileTag>()) {

                auto&t = e -> getComponent<Transform>();
                // Destroy entity if it goes out the cam view
                if (
                    t.position.x > cam.view.x + cam.view.w || // Right edge of view
                    t.position.x < cam.view.x || // Left edge
                    t.position.y > cam.view.y + cam.view.h || // Bottom edge
                    t.position.y < cam.view.y // Top edge
                ) {
                    e -> destroy();
                }
            }
        }
    }
};

#endif //TUTORIAL1_DESTRUCTIONSYSTEM_H