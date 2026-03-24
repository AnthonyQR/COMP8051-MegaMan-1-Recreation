//
// Created by antho on 2026-03-24.
//

#include "SceneTransitionDelaySystem.h"

#include "Component.h"
#include "Game.h"

void SceneTransitionDelaySystem::update(std::vector<std::unique_ptr<Entity>>& entities, float dt) {
    for (auto& entity : entities) {
        if (entity->hasComponent<SceneTransitionDelay>()) {
            auto& sceneTransitionDelay = entity->getComponent<SceneTransitionDelay>();
            sceneTransitionDelay.timer -= dt;
            if (sceneTransitionDelay.timer <= 0) {
                Game::onSceneChangeRequest(sceneTransitionDelay.nextScene);
            }
        }
    }
}
