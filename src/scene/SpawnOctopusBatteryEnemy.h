//
// Created by antho on 2026-04-02.
//

#ifndef TUTORIAL1_SPAWNOCTOPUSBATTERYENEMY_H
#define TUTORIAL1_SPAWNOCTOPUSBATTERYENEMY_H
#include "Entity.h"
#include "World.h"

class SpawnOctopusBatteryEnemy {
public:
    static void spawn(World& world);
    static void finishSpawn(World& world, Entity& spawner, bool isVertical);
};

#endif //TUTORIAL1_SPAWNOCTOPUSBATTERYENEMY_H