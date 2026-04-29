//
// Created by antho on 2026-04-28.
//

#ifndef MEGAMAN_SPAWNENEMYDEATHANIMATION_H
#define MEGAMAN_SPAWNENEMYDEATHANIMATION_H
#include "World.h"
#include "manager/AssetManager.h"

class SpawnEnemyDeathAnimation {
public:
    static void spawn(World& world, Entity& entity);
    Animation newDeathAnim = AssetManager::getAnimation("enemyDeath");
};

#endif //MEGAMAN_SPAWNENEMYDEATHANIMATION_H