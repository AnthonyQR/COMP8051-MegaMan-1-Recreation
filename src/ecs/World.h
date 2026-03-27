//
// Created by antho on 2026-01-21.
//

#ifndef TUTORIAL1_WORLD_H
#define TUTORIAL1_WORLD_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "CollisionSystem.h"
#include "Entity.h"
#include "event/EventManager.h"
#include "KeyboardInputSystem.h"
#include "Map.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "system/AnimationSystems/AnimationSystem.h"
#include "AutoFiringSystem.h"
#include "CameraSystem.h"
#include "DamageSystem.h"
#include "DestructionSystem.h"
#include "EventResponseSystem.h"
#include "FollowEntitySystem.h"
#include "GravitySystem.h"
#include "HitKnockbackSystem.h"
#include "InvulnerableWhileNotFiringSystem.h"
#include "IsFiringTimerSystem.h"
#include "MainMenuSystem.h"
#include "SceneTransitionDelaySystem.h"
#include "SpawnTimerSystem.h"
#include "scene/SceneType.h"

class World {
    Map map;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Entity>> deferredEntities;
    MovementSystem movementSystem;
    RenderSystem renderSystem;
    KeyboardInputSystem keyboardInputSystem;
    GravitySystem gravitySystem;
    CollisionSystem collisionSystem;
    AnimationSystem animationSystem;
    CameraSystem cameraSystem;
    EventManager eventManager;
    SpawnTimerSystem spawnTimerSystem;
    FollowEntitySystem followEntitySystem;
    IsFiringTimerSystem isFiringTimerSystem;
    AutoFiringSystem autoFiringSystem;
    DamageSystem damageSystem;
    DestructionSystem destructionSystem;
    EventResponseSystem eventResponseSystem{*this};
    MainMenuSystem mainMenuSystem;
    SceneTransitionDelaySystem sceneTransitionDelaySystem;
    InvulnerableWhileNotFiringSystem invulnerableWhileNotFiringSystem;
    HitKnockbackSystem hitKnockbackSystem;

public:
    World() = default;
    void update(float dt, const SDL_Event& event, SceneType sceneType) {
        if (sceneType == SceneType::Menu) {
            mainMenuSystem.update(entities, event, *this);
            sceneTransitionDelaySystem.update(entities, dt);
            animationSystem.update(entities, dt);
        }
        else {
            keyboardInputSystem.update(entities, event, dt);
            gravitySystem.update(entities, dt);
            hitKnockbackSystem.update(entities, dt);
            movementSystem.update(entities, dt);
            followEntitySystem.update(entities);
            collisionSystem.update(*this);
            animationSystem.update(entities, dt);
            cameraSystem.update(entities);
            spawnTimerSystem.update(entities, dt);
            isFiringTimerSystem.update(entities, dt);
            autoFiringSystem.update(entities, dt);
            invulnerableWhileNotFiringSystem.update(entities);
            damageSystem.update(entities, *this);
            sceneTransitionDelaySystem.update(entities, dt);
            destructionSystem.update(entities, *this);
        }
        synchronizeEntities();
        cleanup();
    }
    void render() {

        for (auto& entity : entities) {
            if (entity->hasComponent<Camera>()) {
                map.draw(entity->getComponent<Camera>());
                break;
            }
        }
        renderSystem.render(entities);
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
            // Using moe so we don't create a copy
            std::move(
                deferredEntities.begin(),
                deferredEntities.end(),
                std::back_inserter(entities)
            );
            // Clearing the creation buffer
            deferredEntities.clear();
        };
    }

    EventManager& getEventManager() {return eventManager;}
    Map& getMap() {return map;}
};

#endif //TUTORIAL1_WORLD_H