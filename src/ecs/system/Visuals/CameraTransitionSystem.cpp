//
// Created by antho on 2026-05-03.
//

#include "CameraTransitionSystem.h"

#include "Component.h"
#include "Game.h"

void CameraTransitionSystem::update(std::vector<std::unique_ptr<Entity> > &entities, float dt) {
    Entity* player = nullptr;
    Entity* cameraEntity = nullptr;
    Entity* transitionEntity = nullptr;

    for (auto& entity: entities) {
        if (entity ->hasComponent<MainPlayer>()) player = entity.get();
        if (entity->hasComponent<Camera>()) cameraEntity = entity.get();
        if (entity->hasComponent<CameraTransition>()) transitionEntity = entity.get();

        if (player != nullptr && cameraEntity != nullptr && transitionEntity != nullptr) {
            break;
        }
    }

    if (player == nullptr) return;
    if (cameraEntity == nullptr) return;
    if (transitionEntity == nullptr) return;
    for (auto& entity: entities) {
        if (entity->hasComponent<DestroyOutOfViewTag>()) {
            entity->destroy();
        }
    }

    auto& playerVelocity = player->getComponent<Velocity>();
    auto& camera = cameraEntity->getComponent<Camera>();
    auto& transition = transitionEntity->getComponent<CameraTransition>();

    if (transition.isPreparingTransition) {
        Game::gameState.isTransitioning = true;
        transition.timer -= dt;
        if (transition.timer <= 0) {
            transition.timer = transition.transitionTime;
            transition.isTransitioning = true;
            transition.isPreparingTransition = false;
        }
    }

    if (transition.isTransitioning) {
        camera.view.x += (transition.cameraMoveDistance.x / transition.transitionTime * dt);
        camera.view.y += (transition.cameraMoveDistance.y / transition.transitionTime * dt);

        playerVelocity.direction = {1, 1};
        playerVelocity.xSpeed = (transition.playerMoveDistance.x / transition.transitionTime);
        playerVelocity.ySpeed = (transition.playerMoveDistance.y / transition.transitionTime);

        transition.timer -= dt;
        if (transition.timer <= 0) {
            transition.timer = transition.endTransitionTime;
            transition.isTransitioning = false;
            transition.isEndingTransition = true;

            playerVelocity.direction = {0., 0};
            playerVelocity.xSpeed = 0;
            playerVelocity.ySpeed = 0;
        }
    }

    if (transition.isEndingTransition) {
        transition.timer -= dt;
        if (transition.timer <= 0) {
            transitionEntity->destroy();
            Game::gameState.isTransitioning = false;
        }
    }

}
