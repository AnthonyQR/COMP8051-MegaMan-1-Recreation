//
// Created by antho on 2026-03-08.
//

#ifndef TUTORIAL1_EVENTRESPONSESYSTEM_H
#define TUTORIAL1_EVENTRESPONSESYSTEM_H
#include <functional>
#include <SDL3/SDL_rect.h>

#include "event/BaseEvent.h"

class Entity;
class World;

class EventResponseSystem {
public:
    EventResponseSystem(World &world);

private:
    // Collisions
    void onCollision(const CollisionEvent& e, const char* otherTag, World& world);
    bool getCollisionEntities(const CollisionEvent& e, const char* otherTag, Entity*& player, Entity *& other);
    std::vector<bool> checkCollisionDirection(const SDL_FRect& colliderA, const SDL_FRect& colliderB);
    // Player Actions
    void onPlayerAction(const PlayerActionEvent& e,
        const std::function<void(Entity* player, PlayerAction action)>& callback);
};

#endif //TUTORIAL1_EVENTRESPONSESYSTEM_H