//
// Created by antho on 2026-04-07.
//

#ifndef TUTORIAL1_SPAWNFLYINGSHELLENEMY_H
#define TUTORIAL1_SPAWNFLYINGSHELLENEMY_H
#include "Component.h"
#include "Entity.h"

class World;

class SpawnFlyingShellEnemy {
public:
    static void spawn(World& world);
    static Entity* finishSpawn(World& world, Transform spawnerTransform);
};

#endif //TUTORIAL1_SPAWNFLYINGSHELLENEMY_H