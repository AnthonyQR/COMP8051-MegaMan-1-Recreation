//
// Created by antho on 2026-04-03.
//

#include "SpawnSuperCutterEnemy.h"

#include "World.h"
#include "manager/AssetManager.h"

void SpawnSuperCutterEnemy::spawn(World& world) {
    for (auto& spawnPoint : world.getMap().superCutterSpawnPoints) {
        auto& spawner(world.createEntity());
        Animation anim = AssetManager::getAnimation("superCutter");

        SDL_Texture* projectileTex = TextureManager::load("../Assets/Animations/super_cutter_anim.png");

        SDL_FRect projectileSrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect projectileDst {spawnPoint.x, spawnPoint.y, 48, 60};

        auto& spawnerTransform = spawner.addComponent<Transform>
        (Vector2D(spawnPoint.x - (projectileDst.w / 2), spawnPoint.y), 0.0f, 1.0f);



        auto& projectileStats = spawner.addComponent<ProjectileStats>(-1000.0f, 2, Sprite(projectileTex, projectileSrc, projectileDst),
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
        auto& autoFiring = spawner.addComponent<AutoFiring>(0.0f, std::vector<FiringPattern>
        {
            {Vector2D(0, 1.0f), 0.0f},
        }, true, 0.0f, false);

        auto& superCutterDetection = spawner.addComponent<Collider>("EnemyDetect");
        superCutterDetection.rect.w = 48 * 8.5f;
        superCutterDetection.rect.h = 2000.0f;
        superCutterDetection.xOffset = -superCutterDetection.rect.w / 2;
        superCutterDetection.yOffset = -superCutterDetection.rect.h / 2;

        spawner.addComponent<OnPlayerDetectEnterCallback>([](Entity* spawner, Entity* player) {
            auto& isFiring = spawner->getComponent<IsFiring>();
            auto& autoFiring = spawner->getComponent<AutoFiring>();
            isFiring.firing = true;
            isFiring.timer = isFiring.firingDuration;
            autoFiring.loop = true;
        });

        spawner.addComponent<OnPlayerDetectStayCallback>([](Entity* spawner, Entity* player) {
            auto& autoFiring = spawner->getComponent<AutoFiring>();
            auto& projectileStats = spawner->getComponent<ProjectileStats>();

            auto& spawnerTransform = spawner->getComponent<Transform>();
            auto& playerTransform = player->getComponent<Transform>();
            autoFiring.patterns[0].direction.x = (playerTransform.position.x - spawnerTransform.position.x) / 0.8f
                / projectileStats.projectileSpeed;
        });

        spawner.addComponent<OnPlayerDetectLeaveCallback>([](Entity* spawner, Entity* player) {
            auto& isFiring = spawner->getComponent<IsFiring>();
            auto& autoFiring = spawner->getComponent<AutoFiring>();
            std::cout << isFiring.firing << std::endl;
            isFiring.firing = false;
            autoFiring.loop = false;
        });
    }
}
