//
// Created by antho on 2026-03-29.
//

#include "SpawnBeakEnemy.h"

#include "SpawnEnemyDeathAnimation.h"
#include "manager/AssetManager.h"

void SpawnBeakEnemy::spawn(World &world) {
    for (auto& beakEnemySpawnPoint : world.getMap().beakEnemyRightSpawnPoints) {
        auto& beakSpawner(world.createEntity());
        auto& spawnerTransform = beakSpawner.addComponent<Transform>
        (Vector2D(beakEnemySpawnPoint.x, beakEnemySpawnPoint.y), 0.0f, 1.0f);

        bool isFacingRight = false;
        float initialFiringDelay = 1.5f;
        finishSpawn(world, beakSpawner, isFacingRight, initialFiringDelay);
    }

    for (auto& beakEnemySpawnPoint : world.getMap().beakEnemyRightEarlySpawnPoints) {
        auto& beakSpawner(world.createEntity());
        auto& spawnerTransform = beakSpawner.addComponent<Transform>
        (Vector2D(beakEnemySpawnPoint.x, beakEnemySpawnPoint.y), 0.0f, 1.0f);

        bool isFacingRight = false;
        float initialFiringDelay = .9f;
        finishSpawn(world, beakSpawner, isFacingRight, initialFiringDelay);
    }

    for (auto& beakEnemySpawnPoint : world.getMap().beakEnemyLeftSpawnPoints) {
        auto& beakSpawner(world.createEntity());
        auto& spawnerTransform = beakSpawner.addComponent<Transform>
        (Vector2D(beakEnemySpawnPoint.x, beakEnemySpawnPoint.y), 0.0f, 1.0f);

        bool isFacingRight = true;
        float initialFiringDelay = 1.5f;
        finishSpawn(world, beakSpawner, isFacingRight, initialFiringDelay);
    }
}

void SpawnBeakEnemy::finishSpawn(World &world, Entity& spawner, bool facingRight, float initialFiringDelay) {
    spawner.addComponent<SpawnOnVisible>(false, [&world, facingRight, initialFiringDelay](Transform spawnerTransform) {
        auto& beakEnemy(world.createDeferredEntity());
        auto& beakTransform = beakEnemy.addComponent<Transform>
        (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
        beakEnemy.addComponent<IsFacingRight>(facingRight);

        Animation anim = AssetManager::getAnimation("beak");
        anim.getAnimationClip = BeakAnimationSystem::getAnimationClip;
        beakEnemy.addComponent<Animation>(anim);

        SDL_Texture* beakTex = TextureManager::load("Assets/Animations/beak_anim.png");

        SDL_FRect beakSrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect beakDst {beakTransform.position.x, beakTransform.position.y, 48, 48};

        beakEnemy.addComponent<Sprite>(beakTex, beakSrc, beakDst);

        auto& beakCollider = beakEnemy.addComponent<Collider>("Enemy");
        beakCollider.xOffset = beakDst.w / 2;
        beakCollider.rect.w = beakDst.w / 2;
        beakCollider.rect.h = beakDst.h;

        SDL_Texture* beakProjectileTex = TextureManager::load("Assets/beak_projectile.png");
        SDL_FRect beakProjectileSrc{0, 0, 8, 8};
        SDL_FRect beakProjectileDest{0, 0, 8 * 3, 8 * 3};
        Sprite newProjectileSprite{beakProjectileTex, beakProjectileSrc, beakProjectileDest};

        beakEnemy.addComponent<ProjectileStats>(600.0f, 2, Vector2D(0, 0), Vector2D(beakTransform.position.x, beakTransform.position.y),
            [&world, newProjectileSprite](ProjectileStats stats, Entity beak) {
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

        beakEnemy.addComponent<IsFiring>(false, 2.0f, 0.25f, 0.25f);
        beakEnemy.addComponent<BeakEnemyTag>();
        if (!facingRight) {
            beakEnemy.addComponent<AutoFiring>(1.5f, std::vector<FiringPattern>
            {
                {Vector2D(-1, -1.0f).normalize(), 0.25f},
                {Vector2D(-1, -0.2f).normalize(), 0.5f},
                {Vector2D(-1, 0.2f).normalize(), 0.5f},
                {Vector2D(-1, 1.0f).normalize(), 0.5f}
            }, true, false, initialFiringDelay);
        }
        else {
            beakEnemy.addComponent<AutoFiring>(1.5f, std::vector<FiringPattern>
            {
                {Vector2D(1, -1.0f).normalize(), 0.25f},
                {Vector2D(1, -0.2f).normalize(), 0.5f},
                {Vector2D(1, 0.2f).normalize(), 0.5f},
                {Vector2D(1, 1.0f).normalize(), 0.5f}
            }, true, false, initialFiringDelay);
        }

        beakEnemy.addComponent<Health>(1);
        beakEnemy.addComponent<ContactDamage>(1);
        beakEnemy.addComponent<Invulnerability>(false);
        beakEnemy.addComponent<InvulnerableWhileNotFiring>();

        beakEnemy.addComponent<SpawnedEnemyTag>();
        beakEnemy.addComponent<DestroyOutOfViewTag>();

        auto& beakHurtbox(world.createEntity());
        auto& beakHurtboxCollider = beakHurtbox.addComponent<Collider>("Enemy");
        beakHurtboxCollider.rect.w = beakCollider.rect.w;
        beakHurtboxCollider.rect.h = beakCollider.rect.h;
        beakHurtbox.addComponent<Hurtbox>();
        beakHurtbox.addComponent<Transform>(beakTransform);
        beakHurtbox.addComponent<Parent>(&beakEnemy);
        beakHurtbox.addComponent<FollowParent>();

        beakEnemy.addComponent<OnDeathCallback>([&world](Entity* beak) {
            SpawnEnemyDeathAnimation::spawn(world, *beak);
        });

        std::vector newChildren = {&beakHurtbox};
        beakEnemy.addComponent<Children>(newChildren);

        return &beakEnemy;
    });
}
