//
// Created by antho on 2026-04-02.
//

#include "SpawnOctopusBatteryEnemy.h"

#include "SpawnEnemyDeathAnimation.h"
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
        anim.getAnimationClip = OctopusBatteryAnimationSystem::getAnimationClip;
        octopusEnemy.addComponent<Animation>(anim);

        SDL_Texture* bladerTex = TextureManager::load("Assets/Animations/octopus_battery_anim.png");

        SDL_FRect bladerSrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect bladerDst {octopusTransform.position.x, octopusTransform.position.y, 48, 48};

        octopusEnemy.addComponent<Sprite>(bladerTex, bladerSrc, bladerDst);

        auto& octopusCollider = octopusEnemy.addComponent<Collider>("Enemy");
        octopusCollider.rect.w = bladerDst.w;
        octopusCollider.rect.h = bladerDst.h;

        octopusEnemy.addComponent<WallCollidable>(true);

        octopusEnemy.addComponent<OctopusBatteryStats>(350.0f, 1.25f, 1.25f);

        octopusEnemy.addComponent<Health>(5);
        octopusEnemy.addComponent<ContactDamage>(4);

        octopusEnemy.addComponent<SpawnedEnemyTag>();
        octopusEnemy.addComponent<DestroyOutOfViewTag>();

        auto& octopusHurtbox(world.createEntity());
        auto& octopusHurtboxCollider = octopusHurtbox.addComponent<Collider>("Enemy");
        octopusHurtboxCollider.rect.w = octopusCollider.rect.w;
        octopusHurtboxCollider.rect.h = octopusCollider.rect.h;
        octopusHurtbox.addComponent<Hurtbox>();
        octopusHurtbox.addComponent<Transform>(octopusTransform);
        octopusHurtbox.addComponent<Parent>(&octopusEnemy);
        octopusHurtbox.addComponent<FollowParent>();

        octopusEnemy.addComponent<OnDeathCallback>([&world] (Entity* octopus) {
            SpawnEnemyDeathAnimation::spawn(world, *octopus);
        });

        std::vector newChildren = {&octopusHurtbox};
        octopusEnemy.addComponent<Children>(newChildren);

        octopusEnemy.addComponent<OctopusBatteryTag>();

        return &octopusEnemy;
    });
}
