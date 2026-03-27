//
// Created by antho on 2026-03-27.
//

#ifndef TUTORIAL1_FLASHWHILEINVULNERABLESYSTEM_H
#define TUTORIAL1_FLASHWHILEINVULNERABLESYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class FlashWhileInvulnerableSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<FlashWhileInvulnerable>() &&
                entity->hasComponent<Sprite>() &&
                entity->hasComponent<Invulnerability>()) {
                auto& flash = entity->getComponent<FlashWhileInvulnerable>();
                auto& sprite = entity->getComponent<Sprite>();
                auto& invulnerability = entity->getComponent<Invulnerability>();

                if (invulnerability.isInvulnerable) {
                    flash.timer -= dt;
                    if (flash.timer <= 0) {
                        sprite.isVisible = !sprite.isVisible;
                        flash.timer = flash.flashInterval;
                    }
                }
                else {
                    sprite.isVisible = true;
                }
            }
        }
    }
};

#endif //TUTORIAL1_FLASHWHILEINVULNERABLESYSTEM_H