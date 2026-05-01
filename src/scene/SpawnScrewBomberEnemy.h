//
// Created by antho on 2026-04-30.
//

#ifndef MEGAMAN_SPAWNSCREWBOMBERENEMY_H
#define MEGAMAN_SPAWNSCREWBOMBERENEMY_H
#include "World.h"

class SpawnScrewBomberEnemy {
public:
    static void spawn(World& world);
    static void finishSpawn(World& world, Entity& spawner, bool isOnCeiling);
};

#endif //MEGAMAN_SPAWNSCREWBOMBERENEMY_H