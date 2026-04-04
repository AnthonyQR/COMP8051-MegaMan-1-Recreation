//
// Created by antho on 2026-04-03.
//

#include "SpawnSuperCutterEnemy.h"

#include "World.h"
#include "manager/AssetManager.h"

void SpawnSuperCutterEnemy::spawn(World& world) {
    for (auto& spawnPoint : world.getMap().superCutterSpawnPoints) {
        auto& spawner(world.createEntity());
        auto& spawnerTransform = spawner.addComponent<Transform>
        (Vector2D(spawnPoint.x, spawnPoint.y), 0.0f, 1.0f);

        Animation anim = AssetManager::getAnimation("superCutter");

        SDL_Texture* projectileTex = TextureManager::load("../Assets/Animations/super_cutter_anim.png");

        SDL_FRect projectileSrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect projectileDst {spawnerTransform.position.x, spawnerTransform.position.y, 48, 60};

        spawner.addComponent<ProjectileStats>(-1000.0f, 2, Sprite(projectileTex, projectileSrc, projectileDst),
        Vector2D(0, 0), Vector2D(spawnerTransform.position.x, spawnerTransform.position.y),
        [&world, anim](ProjectileStats stats) {
            auto& projectile = world.createDeferredEntity();
            auto& projectileTransform = projectile.addComponent<Transform>(stats.spawnPoint, 0.0f, 1.0f);
            auto& projectileVelocity = projectile.addComponent<Velocity>(stats.direction, stats.projectileSpeed, stats.projectileSpeed);
            auto& projectileGravity = projectile.addComponent<Gravity>(3000.0f, 3000.0f, true);

            auto& projectileAnimation = projectile.addComponent<Animation>(anim);
            auto& projectileSprite = projectile.addComponent<Sprite>(stats.sprite);

            auto& projectileCollider = projectile.addComponent<Collider>("Projectile");
            projectileCollider.rect.w = projectileSprite.dst.w;
            projectileCollider.rect.h = projectileSprite.dst.h;

            projectile.addComponent<ProjectileTag>();
            projectile.addComponent<Health>(5);

            projectile.addComponent<ProjectileDamage>(stats.damage);
            projectile.addComponent<DestroyOutOfViewTag>();
            projectile.addComponent<SuperCutterTag>();
        });

        spawner.addComponent<IsFiring>(false, 0.5f, 0.0f, 0.0f);
        spawner.addComponent<AutoFiring>(0.0f, std::vector<FiringPattern>
        {
            {Vector2D(0, 1.0f), 0.0f},
        }, true, 0.0f);

        auto& superCutterDetection = spawner.addComponent<Collider>("EnemyDetect");;
        superCutterDetection.rect.w = 48 * 10;
        superCutterDetection.rect.h = 2000.0f;
        superCutterDetection.xOffset = superCutterDetection.rect.w / 2;
        superCutterDetection.yOffset = superCutterDetection.rect.h / 2;
        spawner.addComponent<OnPlayerDetectCallback>([](Entity* spawner, Entity* player) {

        });
    }
}
