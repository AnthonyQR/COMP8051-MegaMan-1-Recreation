//
// Created by antho on 2026-03-08.
//

#include "EventResponseSystem.h"

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
        

        // Check collision below player
        if ((playerCollider.y + playerCollider.h) >= wallCollider.y) {
            // t.position.y -= (playerCollider.y + playerCollider.h - wallCollider.y);
            t.position.y = t.oldPosition.y;
            v.direction.y = 0;
            playerGravity.gravityEnabled = false;
            isGrounded.grounded = true;
        }

        // Check collision to the left of the player
        if (playerCollider.x <= (wallCollider.x + wallCollider.w)) {
            //t.position.x += (wallCollider.x + wallCollider.w - playerCollider.x);
            t.position.x = t.oldPosition.x;
        }

        // Check collision to the right of the player
        if ((playerCollider.x + playerCollider.w) >= wallCollider.x) {
            // t.position.x -= (playerCollider.x + playerCollider.w - wallCollider.x);
            t.position.x = t.oldPosition.x;
        }

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
