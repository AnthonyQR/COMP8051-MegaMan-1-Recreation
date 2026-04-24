//
// Created by antho on 2026-04-07.
//

#include "UpdateSceneStateSystem.h"

#include "Component.h"
#include "Game.h"

void UpdateSceneStateSystem::update(const std::unique_ptr<Entity> &entity, SceneState& currentState) {
    auto& sceneState = entity->getComponent<SceneState>();
    sceneState.playerHealth = Game::gameState.playerHealth;
    sceneState.playerMaxHealth = Game::gameState.playerMaxHealth;
    sceneState.lives = Game::gameState.lives;
    sceneState.currentCheckpoint = Game::gameState.currentCheckpoint;
    sceneState.isEnding = Game::gameState.isEnding;
    sceneState.isPaused = Game::gameState.isPaused;

    currentState = sceneState;
}
