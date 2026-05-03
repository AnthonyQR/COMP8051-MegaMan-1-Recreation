//
// Created by antho on 2026-05-02.
//

#include "ScreenFreezeTimerSystem.h"

#include "Component.h"
#include "Game.h"

void ScreenFreezeTimerSystem::update(std::vector<std::unique_ptr<Entity> > &entities, float dt) {
    for (auto& entity : entities) {
        if (entity->hasComponent<ScreenFreeze>()) {
            auto& screenFreeze = entity->getComponent<ScreenFreeze>();
            screenFreeze.timer -= dt;
            if (screenFreeze.timer <= 0) {
                Game::gameState.isScreenFreeze = false;
                entity->destroy();
            }
            else {
                Game::gameState.isScreenFreeze = true;
            }
        }
    }
}
