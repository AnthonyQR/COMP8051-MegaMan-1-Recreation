//
// Created by antho on 2026-04-02.
//

#include "SpawnOctopusBatteryEnemy.h"

#include "manager/AssetManager.h"

void SpawnOctopusBatteryEnemy::spawn(World& world) {
    for (auto& spawnPoint : world.getMap().octopusBatteryVerticalSpawnPoints) {
        auto& octopusSpawner(world.createEntity());
        auto& spawnerTransform = octopusSpawner.addComponent<Transform>
        (Vector2D(spawnPoint.x, spawnPoint.y), 0.0f, 1.0f);

        finishSpawn(world, octopusSpawner, true);
    }

    for (auto& spawnPoint : world.getMap().octopusBatteryHorizontalSpawnPoints) {
        auto& octopusSpawner(world.createEntity());
        auto& spawnerTransform = octopusSpawner.addComponent<Transform>
        (Vector2D(spawnPoint.x, spawnPoint.y), 0.0f, 1.0f);

        finishSpawn(world, octopusSpawner, false);
    }
}

void SpawnOctopusBatteryEnemy::finishSpawn(World &world, Entity &spawner, bool isVertical) {
    spawner.addComponent<SpawnOnVisible>(false, [&world, isVertical](Transform spawnerTransform) {
        auto& octopusEnemy(world.createDeferredEntity());
        auto& octopusTransform = octopusEnemy.addComponent<Transform>
            (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);

        if (isVertical) {
            octopusEnemy.addComponent<Velocity>(Vector2D{0, -1}, 0.0f, 350.0f);
        }
        else {
            octopusEnemy.addComponent<Velocity>(Vector2D{-1, 0}, 350.0f, 0.0f);
        }

        Animation anim = AssetManager::getAnimation("octopus");
        octopusEnemy.addComponent<Animation>(anim);

        SDL_Texture* bladerTex = TextureManager::load("../Assets/Animations/octopus_battery_anim.png");

        SDL_FRect bladerSrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect bladerDst {octopusTransform.position.x, octopusTransform.position.y, 48, 48};

        octopusEnemy.addComponent<Sprite>(bladerTex, bladerSrc, bladerDst);

        auto& octopusCollider = octopusEnemy.addComponent<Collider>("Enemy");
        octopusCollider.rect.w = bladerDst.w;
        octopusCollider.rect.h = bladerDst.h;

        octopusEnemy.addComponent<WallCollidable>(true);

        octopusEnemy.addComponent<OctopusBatteryStats>(350.0f, 2.0f, 2.0f);

        octopusEnemy.addComponent<Health>(5);
        octopusEnemy.addComponent<ContactDamage>(4);

        octopusEnemy.addComponent<SpawnedEnemyTag>();
        octopusEnemy.addComponent<DestroyOutOfViewTag>();

        Animation newDeathAnim = AssetManager::getAnimation("enemyDeath");

        octopusEnemy.addComponent<OnDeathCallback>([&world, newDeathAnim] (Entity* octopus) {
            auto& octopusDeath (world.createDeferredEntity());
            auto& octopusTransform = octopus->getComponent<Transform>();
            auto& deathTransform = octopusDeath.addComponent<Transform>
            (Vector2D(octopusTransform.position.x, octopusTransform.position.y), 0.0f, 1.0f);

            auto& deathAnim = octopusDeath.addComponent<Animation>(newDeathAnim);

            SDL_Texture* deathTex = TextureManager::load("../Assets/Animations/enemy_death_anim.png");
            SDL_FRect deathSrc = deathAnim.clips[deathAnim.currentClip].frameIndices[0];
            SDL_FRect deathDst {deathTransform.position.x, deathTransform.position.y, 48, 48};

            octopusDeath.addComponent<Sprite>(deathTex, deathSrc, deathDst);
            octopusDeath.addComponent<EnemyDeathTag>();
        });

        octopusEnemy.addComponent<OctopusBatteryTag>();

        return &octopusEnemy;
    });
}
