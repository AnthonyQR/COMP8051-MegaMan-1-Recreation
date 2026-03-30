//
// Created by antho on 2026-03-29.
//

#ifndef TUTORIAL1_SPAWNBEAKENEMY_H
#define TUTORIAL1_SPAWNBEAKENEMY_H
#include "World.h"

class SpawnBeakEnemy {
public:
    static void spawn(World& world);
    static void finishSpawn(World& world, Entity& spawner, bool facingRight, float initialFiringDelay);
};

#endif //TUTORIAL1_SPAWNBEAKENEMY_H