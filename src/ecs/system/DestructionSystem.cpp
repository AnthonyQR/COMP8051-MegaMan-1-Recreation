//
// Created by antho on 2026-03-19.
//

#include "DestructionSystem.h"

#include "World.h"

void DestructionSystem::update(const std::vector<std::unique_ptr<Entity>>& entities, World& world) {
    {
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
                    world.getEventManager().emit(DestroyedEvent{e.get()});
                    e -> destroy();
                }
            }
        }
    }
}
