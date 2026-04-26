//
// Created by antho on 2026-01-21.
//

#ifndef TUTORIAL1_WORLD_H
#define TUTORIAL1_WORLD_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "system/ERS/CollisionSystem.h"
#include "Entity.h"
#include "event/EventManager.h"
#include "system/Player/KeyboardInputSystem.h"
#include "Map.h"
#include "system/Movement/MovementSystem.h"
#include "system/Visuals/RenderSystem.h"
#include "system/Visuals/AnimationSystems/AnimationSystem.h"
#include "system/Combat/AutoFiringSystem.h"
#include "system/Enemy/BladerAttackSystem.h"
#include "system/Visuals/CameraSystem.h"
#include "system/Player/CoyoteTimeSystem.h"
#include "system/Combat/DamageSystem.h"
#include "system/Player/DebugTeleportSystem.h"
#include "system/ERS/DestructionSystem.h"
#include "system/ERS/EventResponseSystem.h"
#include "system/Visuals/FlashTimerSystem.h"
#include "system/Movement/FollowParentSystem.h"
#include "system/Movement/GravitySystem.h"
#include "system/Combat/HitKnockbackSystem.h"
#include "system/Combat/InvulnerabilityTimerSystem.h"
#include "system/Combat/InvulnerableWhileNotFiringSystem.h"
#include "system/Combat/IsFiringTimerSystem.h"
#include "system/Scene/MainMenuSystem.h"
#include "system/Movement/MoveTowardsPlayerSystem.h"
#include "system/Enemy/OctopusBatterySystem.h"
#include "system/Player/PauseSystem.h"
#include "system/Scene/SceneTransitionDelaySystem.h"
#include "system/Enemy/SpawnOnVisibleSystem.h"
#include "system/Enemy/SpawnTimerSystem.h"
#include "system/Enemy/SpawnWhileVisibleTimerSystem.h"
#include "system/Movement/StopMovementWhileFiringSystem.h"
#include "system/Visuals/UIRenderSystem.h"
#include "system/Scene/UpdateSceneStateSystem.h"
#include "event/AudioEventQueue.h"
#include "scene/SceneType.h"

class World {
    Map map;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Entity>> deferredEntities;
    std::unique_ptr<Entity> sceneStateEntity;
    SceneState currentSceneState;

    MovementSystem movementSystem;
    RenderSystem renderSystem;
    KeyboardInputSystem keyboardInputSystem;
    GravitySystem gravitySystem;
    CollisionSystem collisionSystem;
    AnimationSystem animationSystem;
    CameraSystem cameraSystem;
    EventManager eventManager;
    SpawnTimerSystem spawnTimerSystem;
    FollowParentSystem followParentSystem;
    IsFiringTimerSystem isFiringTimerSystem;
    AutoFiringSystem autoFiringSystem;
    DamageSystem damageSystem;
    DestructionSystem destructionSystem;
    EventResponseSystem eventResponseSystem{*this};
    MainMenuSystem mainMenuSystem;
    SceneTransitionDelaySystem sceneTransitionDelaySystem;
    InvulnerableWhileNotFiringSystem invulnerableWhileNotFiringSystem;
    HitKnockbackSystem hitKnockbackSystem;
    InvulnerabilityTimerSystem invulnerabilityTimerSystem;
    FlashTimerSystem flashTimerSystem;
    SpawnOnVisibleSystem spawnOnVisibleSystem;
    AudioEventQueue audioEventQueue;
    CoyoteTimeSystem coyoteTimeSystem;
    UIRenderSystem uiRenderSystem;
    MoveTowardsPlayerSystem moveTowardsPlayerSystem;
    BladerAttackSystem bladerAttackSystem;
    OctopusBatterySystem octopusBatterySystem;
    DebugTeleportSystem debugTeleportSystem;
    UpdateSceneStateSystem updateSceneStateSystem;
    StopMovementWhileFiringSystem stopMovementWhileFiringSystem;
    PauseSystem pauseSystem;
    SpawnWhileVisibleTimerSystem spawnWhileVisibleTimerSystem;

public:
    World() = default;
    void update(float dt, const SDL_Event& event, SceneType sceneType) {
        if (sceneType == SceneType::Menu) {
            mainMenuSystem.update(entities, event, *this);
            sceneTransitionDelaySystem.update(entities, dt);
            animationSystem.update(entities, dt);
        }
        else {
            if (!currentSceneState.isEnding && !currentSceneState.isPaused) {
                debugTeleportSystem.update(entities, event, *this);
                keyboardInputSystem.update(entities, event, dt);

                animationSystem.update(entities, dt);

                spawnTimerSystem.update(entities, dt);
                spawnOnVisibleSystem.update(entities);
                spawnWhileVisibleTimerSystem.update(entities, dt);
                isFiringTimerSystem.update(entities, dt);
                autoFiringSystem.update(entities, dt);

                invulnerableWhileNotFiringSystem.update(entities);
                invulnerabilityTimerSystem.update(entities, dt);
                flashTimerSystem.update(entities, dt);

                damageSystem.update(entities, *this);
                destructionSystem.update(entities, *this);
            }

            if (!currentSceneState.isEnding) {
                pauseSystem.update(entities, event, *this);
            }
            sceneTransitionDelaySystem.update(entities, dt);
            updateSceneStateSystem.update(sceneStateEntity, currentSceneState);
        }
        audioEventQueue.process(); // Process all the audio events
        synchronizeEntities();
        cleanup();
    }

    void fixedUpdate (float dt, const SDL_Event& event, SceneType sceneType) {
        if (sceneType == SceneType::Menu) {
            // Currently Do Nothing
        }
        else {
            if (!currentSceneState.isEnding && !currentSceneState.isPaused) {
                coyoteTimeSystem.update(entities, dt);
                gravitySystem.update(entities, dt);
                hitKnockbackSystem.update(entities, dt);
                moveTowardsPlayerSystem.update(entities);

                bladerAttackSystem.update(entities, dt);
                octopusBatterySystem.update(entities, dt);
                stopMovementWhileFiringSystem.update(entities);

                movementSystem.update(entities, dt);
                followParentSystem.update(entities);

                collisionSystem.update(*this);
                cameraSystem.update(entities);
            }
        }
    }
    void render() {

        for (auto& entity : entities) {
            if (entity->hasComponent<Camera>()) {
                map.draw(entity->getComponent<Camera>());
                break;
            }
        }
        renderSystem.render(entities);
        uiRenderSystem.render(entities);
    }

    Entity& createEntity() {
        // We use emplace instead of push so we don't create a copy
        entities.emplace_back(std::make_unique<Entity>());
        return *entities.back();
    }

    Entity& createDeferredEntity() {
        deferredEntities.emplace_back(std::make_unique<Entity>());
        return *deferredEntities.back();
    }

    std::vector<std::unique_ptr<Entity>>& getEntities() {
        return entities;
    }

    void cleanup() {
        // Cleanup collision keys
        for (auto & entity : entities) {
            if (!entity->isActive()) {
                collisionSystem.cleanupDestroyedCollisions(entity.get());
            }
        }

        for (auto & entity : entities) {
            if (!entity->isActive() && entity->hasComponent<Children>()) {
                for (auto& child : entity->getComponent<Children>().children) {
                    child->destroy();
                }
            }
        }

        // Use a lambda predicate to remove all inactive entities
        std::erase_if(
            entities,
            [](std::unique_ptr<Entity>& entity)
            {
                return !entity->isActive();
            }
        );
    }

    void synchronizeEntities() {
        if (!deferredEntities.empty()) {
            // Push back all deferred entities to the entities vector
            // Using move so we don't create a copy
            std::move(
                deferredEntities.begin(),
                deferredEntities.end(),
                std::back_inserter(entities)
            );
            // Clearing the creation buffer
            deferredEntities.clear();
        };
    }

    Entity& CreateSceneStateEntity() {
        sceneStateEntity = std::make_unique<Entity>();
        return *sceneStateEntity;
    }

    EventManager& getEventManager() {return eventManager;}
    AudioEventQueue& getAudioEventQueue() {return audioEventQueue;}
    Map& getMap() {return map;}
};

#endif //TUTORIAL1_WORLD_H