//
// Created by antho on 2026-05-02.
//

#ifndef MEGAMAN_ONITEMCOLLISIONEVENT_H
#define MEGAMAN_ONITEMCOLLISIONEVENT_H
#include "Entity.h"
#include "World.h"
#include "event/BaseEvent.h"

class OnItemCollisionEvent {
public:
    static void onCollision(Entity *enemy, Entity *other,
        const CollisionEvent& e, const char* otherTag, World& world);
};

#endif //MEGAMAN_ONITEMCOLLISIONEVENT_H