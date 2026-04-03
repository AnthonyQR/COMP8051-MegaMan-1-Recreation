//
// Created by antho on 2026-04-02.
//

#ifndef TUTORIAL1_ONENEMYCOLLISIONEVENT_H
#define TUTORIAL1_ONENEMYCOLLISIONEVENT_H
#include "Entity.h"
#include "World.h"

class OnEnemyCollisionEvent {
public:
    static void onCollision(Entity *enemy, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
};

#endif //TUTORIAL1_ONENEMYCOLLISIONEVENT_H