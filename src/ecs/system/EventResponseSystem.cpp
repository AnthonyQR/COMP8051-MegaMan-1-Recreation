//
// Created by antho on 2026-03-08.
//

#include "EventResponseSystem.h"

#include <list>

#include "Game.h"
#include "World.h"

EventResponseSystem::EventResponseSystem(World &world) {
    // Subscriptions
    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::Collision) return;
            const auto& collision = static_cast<const CollisionEvent&>(e); // Cast base type to collision type

            onCollision(collision, "Item", world);
            onCollision(collision, "Wall", world);
            onCollision(collision, "Projectile", world);
        }
    );

    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::PlayerAction) return;
            const auto& playerAction = static_cast<const PlayerActionEvent&>(e);
            // TODO onPlayerAction
        }
    );
}

void EventResponseSystem::onCollision(const CollisionEvent& e, const char* otherTag, World& world) {
    Entity* player = nullptr;
    Entity* other = nullptr;

    if (!getCollisionEntities(e, otherTag, player, other)) return;

    if (std::string(otherTag) == "Item") {
        if (e.state != CollisionState::Enter) return;

        other->destroy();

        for (auto& entity: world.getEntities()) {
            if (!entity->hasComponent<SceneState>()) continue;

            // Scene State
            auto& sceneState = entity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            if (sceneState.coinsCollected > 1) {
                Game::onSceneChangeRequest("level2");
            }
        }
    }

    else if (std::string(otherTag) == "Wall") {
        if (e.state !=CollisionState::Stay) return;

        // Stop the player
        auto& t = player->getComponent<Transform>();
        auto& v = player->getComponent<Velocity>();
        auto& playerGravity = player->getComponent<Gravity>();
        auto& isGrounded = player->getComponent<IsGrounded>();
        // t.position = t.oldPosition;

        auto& playerCollider = player->getComponent<Collider>().rect;
        auto& wallCollider = other->getComponent<Collider>().rect;

        if (e.axis == CollisionAxis::Horizontal) {
            std::cout << "Horizontal" << std::endl;

            float leftPenetrationDepth = playerCollider.x - (wallCollider.x + wallCollider.w);
            float rightPenetrationDepth = (playerCollider.x + playerCollider.w) - wallCollider.x;
            if (std::abs(leftPenetrationDepth) < std::abs(rightPenetrationDepth)) {
                t.position.x = (wallCollider.x + wallCollider.w + 0.1f);
            }
            else {
                t.position.x = (wallCollider.x - playerCollider.w - 0.1f);
            }
            playerCollider.x = t.position.x;

        }

        else if (e.axis == CollisionAxis::Vertical) {
            t.position.y = t.oldPosition.y;
            v.direction.y = 0;
            float topPenetrationDepth = playerCollider.y - (wallCollider.y + wallCollider.h);
            float bottomPenetrationDepth = (playerCollider.y + playerCollider.h) - wallCollider.y;
            if (std::abs(bottomPenetrationDepth) < std::abs(topPenetrationDepth)) {
                t.position.y = wallCollider.y - playerCollider.h - 0.1f;
                v.direction.y = 0;
                isGrounded.grounded = true;
            }
            else {
                t.position.y = (wallCollider.y + wallCollider.h + 0.1f);
                v.direction.y = 0;
            }
            playerCollider.y = t.position.y;

        }


        /*
        // Collision to the left or right of the player
        if (collisions.at(2) && !collisions.at(3)){
            t.position.x += (wallCollider.x + wallCollider.w - playerCollider.x);
        }

        if (collisions.at(3) && !collisions.at(2)) {
            t.position.x -= (playerCollider.x + playerCollider.w - wallCollider.x);
        }

        // Collision below the player
        if (collisions.at(1) && v.direction.y > 0) {
            t.position.y -= (playerCollider.y + playerCollider.h - wallCollider.y);
            v.direction.y = 0;
            isGrounded.grounded = true;
            playerGravity.gravityEnabled = false;
        }

        // Collision above the player
        if (collisions.at(0) && v.direction.y < 0) {
            t.position.y = t.oldPosition.y;
            v.direction.y = 0;
        }
        */

    }

    else if (std::string(otherTag) == "Projectile") {
        if (e.state !=CollisionState::Enter) return;

        // This logic is simple and direct
        // Ideally we would only operate on data in an update function
        // (Hinting at transient entities)
        auto& health = player->getComponent<Health>();
        health.currentHealth--;

        Game::gameState.playerHealth = health.currentHealth;

        std::cout << health.currentHealth << std::endl;

        if (health.currentHealth <= 0) {
            // Destroy Player
            player->destroy();

            // Change scenes deferred
            Game::onSceneChangeRequest("gameover");
        }
    }
}

std::vector<bool> EventResponseSystem::checkCollisionDirection(const SDL_FRect& colliderA, const SDL_FRect& colliderB) {
    std::vector<bool>collisions = {false, false, false, false};

    // Check collision above player
    if ((colliderA.y <= colliderB.y + colliderB.h)) {
        collisions.at(0) = true;
    }

    // Check collision below player
    if ((colliderA.y + colliderA.h >= colliderB.y)) {
        // t.position.y -= (playerCollider.y + playerCollider.h - wallCollider.y);
        collisions.at(1) = true;
    }

    // Check collision to the left of the player
    if ((colliderA.x <= colliderB.x + colliderB.w)) {
        //t.position.x += (wallCollider.x + wallCollider.w - playerCollider.x);
        std::cout << "left" << std::endl;
        collisions.at(2) = true;
    }

    // Check collision to the right of the player
    if (colliderA.x + colliderA.w >= colliderB.x) {
        // t.position.x -= (playerCollider.x + playerCollider.w - wallCollider.x);
        std::cout << "right" << std::endl;
        collisions.at(3) = true;
    }

    return collisions;
}

bool EventResponseSystem::getCollisionEntities(
    const CollisionEvent &e,
    const char *otherTag,
    Entity *&player,
    Entity *&other
) {
    if (e.entityA == nullptr || e.entityB == nullptr) return false;

    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>())) return false;

    auto& colliderA = e.entityA->getComponent<Collider>();
    auto& colliderB = e.entityB->getComponent<Collider>();

    if (colliderA.tag == "Player" && colliderB.tag == otherTag) {
        player = e.entityA;
        other = e.entityB;
    }
    else if (colliderA.tag == otherTag && colliderB.tag == "Player") {
        player = e.entityB;
        other = e.entityA;
    }

    return player && other;
}
