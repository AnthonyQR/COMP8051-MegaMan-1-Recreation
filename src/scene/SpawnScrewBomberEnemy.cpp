//
// Created by antho on 2026-04-30.
//

#include "SpawnScrewBomberEnemy.h"

#include "SpawnEnemyDeathAnimation.h"
#include "manager/AssetManager.h"

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
    spawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
        auto& screwBomberEnemy(world.createDeferredEntity());
        auto& screwBomberTransform = screwBomberEnemy.addComponent<Transform>
        (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);

        Animation anim = AssetManager::getAnimation("screwBomber");
        //anim.getAnimationClip = screwBomberAnimationSystem::getAnimationClip;
        screwBomberEnemy.addComponent<Animation>(anim);

        SDL_Texture* screwBomberTex = TextureManager::load("Assets/Animations/screw_bomber_anim.png");

        SDL_FRect screwBomberSrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect screwBomberDst {screwBomberTransform.position.x, screwBomberTransform.position.y, 48, 48};

        screwBomberEnemy.addComponent<Sprite>(screwBomberTex, screwBomberSrc, screwBomberDst);

        auto& screwBomberCollider = screwBomberEnemy.addComponent<Collider>("Enemy");
        screwBomberCollider.rect.w = screwBomberDst.w;
        screwBomberCollider.rect.h = screwBomberDst.h;

        SDL_Texture* screwBomberProjectileTex = TextureManager::load("Assets/screw_bomber_projectile.png");
        SDL_FRect screwBomberProjectileSrc{0, 0, 6, 6};
        SDL_FRect screwBomberProjectileDest{0, 0, 6 * 3, 6 * 3};
        Sprite newProjectileSprite{screwBomberProjectileTex, screwBomberProjectileSrc, screwBomberProjectileDest};

        screwBomberEnemy.addComponent<ProjectileStats>(600.0f, 2, Vector2D(0, 0),
            Vector2D(screwBomberTransform.position.x, screwBomberTransform.position.y),
            [&world, newProjectileSprite](ProjectileStats stats, Entity screwBomber) {
            auto& projectile = world.createDeferredEntity();
            auto& projectileTransform = projectile.addComponent<Transform>(stats.spawnPoint, 0.0f, 1.0f);
            auto& projectileSprite = projectile.addComponent<Sprite>(newProjectileSprite);

            auto& projectileCollider = projectile.addComponent<Collider>("Projectile");
            projectileCollider.rect.w = projectileSprite.dst.w;
            projectileCollider.rect.h = projectileSprite.dst.h;

            projectile.addComponent<ProjectileTag>();
            auto& projectileVelocity = projectile.addComponent<Velocity>(stats.direction, stats.projectileSpeed, stats.projectileSpeed);

            projectile.addComponent<ProjectileDamage>(stats.damage);
            projectile.addComponent<DestroyOutOfViewTag>();
            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("enemyShoot"));
        }
    );

        screwBomberEnemy.addComponent<IsFiring>(false, 2.0f, 0.25f, 0.25f);


        screwBomberEnemy.addComponent<OnPlayerDetectEnterCallback>([](Entity* screwBomberEnemy, Entity* player) {
            auto& isFiring = screwBomberEnemy->getComponent<IsFiring>();
            auto& autoFiring = screwBomberEnemy->getComponent<AutoFiring>();
            isFiring.firing = true;
            isFiring.timer = isFiring.firingDuration;
            autoFiring.loop = true;
        });

        screwBomberEnemy.addComponent<OnPlayerDetectLeaveCallback>([](Entity* screwBomberEnemy, Entity* player) {
            auto& isFiring = screwBomberEnemy->getComponent<IsFiring>();
            auto& autoFiring = screwBomberEnemy->getComponent<AutoFiring>();
            isFiring.firing = false;
            autoFiring.loop = false;
        });

        screwBomberEnemy.addComponent<Health>(3);
        screwBomberEnemy.addComponent<ContactDamage>(1);

        screwBomberEnemy.addComponent<SpawnedEnemyTag>();
        screwBomberEnemy.addComponent<DestroyOutOfViewTag>();

        auto& screwBomberHurtbox(world.createEntity());
        auto& screwBomberHurtboxCollider = screwBomberHurtbox.addComponent<Collider>("Enemy");
        screwBomberHurtboxCollider.rect.w = screwBomberCollider.rect.w;
        screwBomberHurtboxCollider.rect.h = screwBomberCollider.rect.h;
        screwBomberHurtbox.addComponent<Hurtbox>();
        screwBomberHurtbox.addComponent<Transform>(screwBomberTransform);
        screwBomberHurtbox.addComponent<Parent>(&screwBomberEnemy);
        screwBomberHurtbox.addComponent<FollowParent>();

        screwBomberEnemy.addComponent<OnDeathCallback>([&world](Entity* screwBomber) {
            SpawnEnemyDeathAnimation::spawn(world, *screwBomber);
        });

        std::vector newChildren = {&screwBomberHurtbox};
        screwBomberEnemy.addComponent<Children>(newChildren);
        return &screwBomberEnemy;
    });
}