//
// Created by antho on 2026-04-02.
//

#ifndef TUTORIAL1_ONPLAYERCOLLISIONEVENT_H
#define TUTORIAL1_ONPLAYERCOLLISIONEVENT_H
#include "World.h"

class OnPlayerCollisionEvent {
public:
    static void onCollision(Entity *player, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);

    static void wallCollision(Entity *player, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
    static void ladderCollision(Entity *player, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
    static void cameraBoundsCollision(Entity *player, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
    static void enemyCollision(Entity *player, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
    static void projectileCollision(Entity *player, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
    static void deathCollision(Entity *player, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
    static void enemyDetectCollision(Entity *player, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
};

#endif //TUTORIAL1_ONPLAYERCOLLISIONEVENT_H