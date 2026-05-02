//
// Created by antho on 2026-05-01.
//

#ifndef MEGAMAN_SPAWNITEM_H
#define MEGAMAN_SPAWNITEM_H
#include "World.h"

class SpawnItem {
public:
    static void spawn(World& world, Entity& entity);
    static void spawnSmallHealth(World& world, Entity& entity);
    static void spawnLargeHealth(World& world, Entity& entity);
    static void spawnOneUp(World& world, Entity& entity);
};


#endif //MEGAMAN_SPAWNITEM_H