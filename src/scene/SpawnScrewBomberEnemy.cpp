//
// Created by antho on 2026-04-30.
//

#include "SpawnScrewBomberEnemy.h"

void SpawnScrewBomberEnemy::spawn(World &world) {
    for (auto& screwBomberEnemySpawnPoint : world.getMap().screwBomberBottomSpawnPoints) {
        auto& screwBomberSpawner(world.createEntity());
        auto& spawnerTransform = screwBomberSpawner.addComponent<Transform>
        (Vector2D(screwBomberEnemySpawnPoint.x, screwBomberEnemySpawnPoint.y), 0.0f, 1.0f);

        bool isOnCeiling = false;
        finishSpawn(world, screwBomberSpawner, isOnCeiling);
    }

    for (auto& screwBomberEnemySpawnPoint : world.getMap().screwBomberTopSpawnPoints) {
        auto& screwBomberSpawner(world.createEntity());
        auto& spawnerTransform = screwBomberSpawner.addComponent<Transform>
        (Vector2D(screwBomberEnemySpawnPoint.x, screwBomberEnemySpawnPoint.y), 0.0f, 1.0f);

        bool isOnCeiling = true;
        finishSpawn(world, screwBomberSpawner, isOnCeiling);
    }
}

void SpawnScrewBomberEnemy::finishSpawn(World &world, Entity &spawner, bool isOnCeiling) {

}
