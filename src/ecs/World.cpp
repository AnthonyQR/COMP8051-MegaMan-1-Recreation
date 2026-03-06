//
// Created by antho on 2026-02-04.
//

#include "World.h"

#include <iostream>

#include "Game.h"

void Test(const CollisionEvent& collision) {
    if (collision.entityA == nullptr || collision.entityB == nullptr) return;
    if (!(collision.entityA->hasComponent<Collider>() && collision.entityB->hasComponent<Collider>())) return;

    auto& colliderA = collision.entityA->getComponent<Collider>();
    auto& colliderB = collision.entityB->getComponent<Collider>();
    std::cout << "A collision occurred between " << colliderA.tag << " and " << colliderB.tag << "." << std::endl;
}

World::World() {
    // Subscribe to the collision events
    eventManager.subscribe([this](const CollisionEvent& collision) {
        Entity* sceneStateEntity = nullptr;

        // Find Scene State
        for (auto& e : entities) {
            if (e -> hasComponent<SceneState>()) {
                sceneStateEntity = e.get();
                break;
            }
        }

        if (!sceneStateEntity) return;

        if (collision.entityA == nullptr || collision.entityB == nullptr) return;

        if (!(collision.entityA->hasComponent<Collider>() && collision.entityB->hasComponent<Collider>())) return;

        auto& colliderA = collision.entityA->getComponent<Collider>();
        auto& colliderB = collision.entityB->getComponent<Collider>();

        Entity* player = nullptr;
        Entity* item = nullptr;
        Entity* wall = nullptr;
        Entity* projectile = nullptr;

        if (colliderA.tag == "Player" && colliderB.tag == "Item") {
            player = collision.entityA;
            item = collision.entityB;
        }
        else if (colliderA.tag == "Item" && colliderB.tag == "Player") {
            player = collision.entityB;
            item = collision.entityA;
        }

        if (player && item) {
            item->destroy();

            // Scene State
            auto& sceneState = sceneStateEntity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            if (sceneState.coinsCollected > 1) {
                Game::onSceneChangeRequest("level2");
            }
        }

        // Player vs Wall
        if (colliderA.tag == "Player" && colliderB.tag == "Wall") {
            player = collision.entityA;
            wall = collision.entityB;
        }
        else if (colliderA.tag == "Wall" && colliderB.tag == "Player") {
            player = collision.entityB;
            wall = collision.entityA;
        }

        if (player && wall) {
            // Stop the player
            auto& t = player->getComponent<Transform>();
            t.position = t.oldPosition;
        }

        // Player vs Projectile
        if (colliderA.tag == "Player" && colliderB.tag == "Projectile") {
            player = collision.entityA;
            projectile = collision.entityB;
        }
        else if (colliderA.tag == "Projectile" && colliderB.tag == "Player") {
            player = collision.entityB;
            projectile = collision.entityA;
        }

        if (player && projectile) {
            // Destroy Player
            player->destroy();

            // Change scenes deferred
            Game::onSceneChangeRequest("gameover");
        }

    });


    eventManager.subscribe(Test);
}


