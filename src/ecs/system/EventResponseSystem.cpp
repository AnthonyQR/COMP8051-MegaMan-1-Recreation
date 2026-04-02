//
// Created by antho on 2026-03-08.
//

#include "EventResponseSystem.h"

#include <list>

#include "Game.h"
#include "World.h"
#include "ERS/OnDestroyEvent.h"
#include "ERS/OnPlayerCollisionEvent.h"

EventResponseSystem::EventResponseSystem(World &world) {
    // Subscriptions
    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::Collision) return;
            const auto& collision = static_cast<const CollisionEvent&>(e); // Cast base type to collision type

            onCollision(collision, "Item", world);
            onCollision(collision, "Wall", world);
            onCollision(collision, "Projectile", world);
            onCollision(collision, "Ladder", world);
            onCollision(collision, "Camera Bounds", world);
            onCollision(collision, "Enemy", world);
            onCollision(collision, "Death", world);
            onCollision(collision, "EnemyDetect", world);
        }
    );

    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::PlayerAction) return;
            const auto& playerAction = static_cast<const PlayerActionEvent&>(e);
            // TODO onPlayerAction
        }
    );

    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::Destroyed) return;
            const auto& destroyed = static_cast<const DestroyedEvent&>(e);
            OnDestroyEvent::onDestroy(destroyed, world);
        }
    );
}

void EventResponseSystem::onCollision(const CollisionEvent& e, const char* otherTag, World& world) {
    Entity* player = nullptr;
    Entity* other = nullptr;

    if (e.entityA == nullptr || e.entityB == nullptr) return;

    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>())) return;

    auto& colliderA = e.entityA->getComponent<Collider>();
    auto& colliderB = e.entityB->getComponent<Collider>();

    if (colliderA.tag == "Player" && colliderB.tag == otherTag) {
        player = e.entityA;
        other = e.entityB;
        OnPlayerCollisionEvent::onCollision(player, other, e, otherTag, world);
    }
    else if (colliderA.tag == otherTag && colliderB.tag == "Player") {
        player = e.entityB;
        other = e.entityA;
        OnPlayerCollisionEvent::onCollision(player, other, e, otherTag, world);
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

void EventResponseSystem::onPlayerAction(const PlayerActionEvent &e,
    const std::function<void(Entity *player, PlayerAction action)> &callback) {
    if (e.action == PlayerAction::Attack) {
        return;
    }
}