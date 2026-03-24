//
// Created by antho on 2026-03-23.
//

#include "OnDestroyEvent.h"
#include "World.h"

void OnDestroyEvent::onDestroy(const DestroyedEvent &e, World &world) {
    Entity* destroyedEntity = e.entity;

    if (destroyedEntity->hasComponent<ProjectileTag>() &&
        destroyedEntity->hasComponent<PlayerTag>()) {
        std::vector<std::unique_ptr<Entity>>& entities = world.getEntities();

        for (auto& e : entities) {
            if (e->hasComponent<PlayerTag>() && e->hasComponent<ProjectileLimit>()) {
                auto& projectileLimit = e->getComponent<ProjectileLimit>();
                projectileLimit.currentProjectiles--;
                if (projectileLimit.currentProjectiles <= 0) {
                    projectileLimit.currentProjectiles = 0;
                }
                return;
            }
        }
    }
}

