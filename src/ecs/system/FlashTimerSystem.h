//
// Created by antho on 2026-03-27.
//

#ifndef TUTORIAL1_FLASHWHILEINVULNERABLESYSTEM_H
#define TUTORIAL1_FLASHWHILEINVULNERABLESYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class FlashTimerSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<FlashTimer>() &&
                entity->hasComponent<Sprite>()) {
                auto& flash = entity->getComponent<FlashTimer>();
                auto& sprite = entity->getComponent<Sprite>();

                if (flash.durationTimer > 0) {
                    flash.durationTimer -= dt;
                    flash.intervalTimer -= dt;
                    if (flash.intervalTimer <= 0) {
                        sprite.isVisible = !sprite.isVisible;
                        flash.intervalTimer = flash.flashInterval;
                    }
                }
                else {
                    sprite.isVisible = sprite.normallyVisible;
                }
            }
        }
    }
};

#endif //TUTORIAL1_FLASHWHILEINVULNERABLESYSTEM_H