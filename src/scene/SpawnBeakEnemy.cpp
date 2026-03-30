//
// Created by antho on 2026-03-29.
//

#include "SpawnBeakEnemy.h"

#include "manager/AssetManager.h"

void SpawnBeakEnemy::spawn(World &world) {
    for (auto& beakEnemySpawnPoint : world.getMap().beakEnemyRightSpawnPoints) {
        auto& beakSpawner(world.createEntity());
        auto& spawnerTransform = beakSpawner.addComponent<Transform>
        (Vector2D(beakEnemySpawnPoint.x, beakEnemySpawnPoint.y), 0.0f, 1.0f);

        beakSpawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
            auto& beakEnemy(world.createDeferredEntity());
            auto& beakTransform = beakEnemy.addComponent<Transform>
            (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
            beakEnemy.addComponent<IsFacingRight>(false);

            Animation anim = AssetManager::getAnimation("beak");
            beakEnemy.addComponent<Animation>(anim);

            SDL_Texture* beakTex = TextureManager::load("../Assets/Animations/beak_anim.png");

            SDL_FRect beakSrc = anim.clips[anim.currentClip].frameIndices[0];
            SDL_FRect beakDst {beakTransform.position.x, beakTransform.position.y, 48, 48};

            beakEnemy.addComponent<Sprite>(beakTex, beakSrc, beakDst);

            auto& beakCollider = beakEnemy.addComponent<Collider>("Enemy");
            beakCollider.xOffset = beakDst.w / 2;
            beakCollider.rect.w = beakDst.w / 2;
            beakCollider.rect.h = beakDst.h;

            SDL_Texture* beakProjectileTex = TextureManager::load("../Assets/beak_projectile.png");
            SDL_FRect beakProjectileSrc{0, 0, 8, 8};
            SDL_FRect beakProjectileDest{0, 0, 8 * 3, 8 * 3};

            beakEnemy.addComponent<ProjectileStats>(600.0f, 2, Sprite(beakProjectileTex, beakProjectileSrc, beakProjectileDest),
            Vector2D(0, 0), Vector2D(beakTransform.position.x, beakTransform.position.y), [&world](ProjectileStats stats) {
                auto& projectile = world.createDeferredEntity();
                auto& projectileTransform = projectile.addComponent<Transform>(stats.spawnPoint, 0.0f, 1.0f);
                auto& projectileSprite = projectile.addComponent<Sprite>(stats.sprite);
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

            beakEnemy.addComponent<IsFiring>(false, 2.25f, 0.25f, 0.25f);
            beakEnemy.addComponent<BeakEnemyTag>();
            beakEnemy.addComponent<AutoFiring>(2.0f, std::vector<FiringPattern>
            {
                {Vector2D(-1, -1.0f).normalize(), 0.25f},
                {Vector2D(-1, -0.2f).normalize(), 0.5f},
                {Vector2D(-1, 0.2f).normalize(), 0.5f},
                {Vector2D(-1, 1.0f).normalize(), 0.5f}
            }, true, 2.0f);

            beakEnemy.addComponent<Health>(1);
            beakEnemy.addComponent<ContactDamage>(1);
            beakEnemy.addComponent<Invulnerability>(false);
            beakEnemy.addComponent<InvulnerableWhileNotFiring>();

            beakEnemy.addComponent<SpawnedEnemyTag>();
            beakEnemy.addComponent<DestroyOutOfViewTag>();
            return &beakEnemy;
        });
    }


    for (auto& beakEnemySpawnPoint : world.getMap().beakEnemyLeftSpawnPoints) {
        auto& beakSpawner(world.createEntity());
        auto& spawnerTransform = beakSpawner.addComponent<Transform>
        (Vector2D(beakEnemySpawnPoint.x, beakEnemySpawnPoint.y), 0.0f, 1.0f);

        beakSpawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
            auto& beakEnemy = world.createDeferredEntity();
            auto& beakTransform = beakEnemy.addComponent<Transform>
            (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
            beakEnemy.addComponent<IsFacingRight>(true);


            Animation anim = AssetManager::getAnimation("beak");
            beakEnemy.addComponent<Animation>(anim);

            SDL_Texture* beakTex = TextureManager::load("../Assets/Animations/beak_anim.png");

            SDL_FRect beakSrc = anim.clips[anim.currentClip].frameIndices[0];
            SDL_FRect beakDst {beakTransform.position.x, beakTransform.position.y, 48, 48};

            beakEnemy.addComponent<Sprite>(beakTex, beakSrc, beakDst);

            auto& beakCollider = beakEnemy.addComponent<Collider>("Enemy");
            beakCollider.rect.w = beakDst.w / 2;
            beakCollider.rect.h = beakDst.h;

            SDL_Texture* beakProjectileTex = TextureManager::load("../Assets/beak_projectile.png");
            SDL_FRect beakProjectileSrc{0, 0, 8, 8};
            SDL_FRect beakProjectileDest{0, 0, 8 * 3, 8 * 3};

            beakEnemy.addComponent<ProjectileStats>(600.0f, 2, Sprite(beakProjectileTex, beakProjectileSrc, beakProjectileDest),
            Vector2D(0, 0), Vector2D(beakTransform.position.x, beakTransform.position.y), [&world](ProjectileStats stats) {
                auto& projectile = world.createDeferredEntity();
                auto& projectileTransform = projectile.addComponent<Transform>(stats.spawnPoint, 0.0f, 1.0f);
                auto& projectileSprite = projectile.addComponent<Sprite>(stats.sprite);
                auto& projectileCollider = projectile.addComponent<Collider>("Projectile");
                projectileCollider.rect.w = projectileSprite.dst.w / 2;
                projectileCollider.rect.h = projectileSprite.dst.h;
                projectile.addComponent<ProjectileTag>();
                auto& projectileVelocity = projectile.addComponent<Velocity>(stats.direction, stats.projectileSpeed, stats.projectileSpeed);
                projectile.addComponent<ProjectileDamage>(stats.damage);
                projectile.addComponent<DestroyOutOfViewTag>();
                world.getAudioEventQueue().push(std::make_unique<AudioEvent>("enemyShoot"));
            }
        );

            beakEnemy.addComponent<IsFiring>(false, 2.25f, 0.25f, 0.25f);
            beakEnemy.addComponent<BeakEnemyTag>();
            beakEnemy.addComponent<AutoFiring>(2.0f, std::vector<FiringPattern>
            {
                {Vector2D(1, -1.0f).normalize(), 0.25f},
                {Vector2D(1, -0.2f).normalize(), 0.5f},
                {Vector2D(1, 0.2f).normalize(), 0.5f},
                {Vector2D(1, 1.0f).normalize(), 0.5f}
            }, true, 2.0f);

            beakEnemy.addComponent<Health>(1);
            beakEnemy.addComponent<ContactDamage>(1);
            beakEnemy.addComponent<Invulnerability>(false);
            beakEnemy.addComponent<InvulnerableWhileNotFiring>();

            beakEnemy.addComponent<SpawnedEnemyTag>();
            beakEnemy.addComponent<DestroyOutOfViewTag>();

            return &beakEnemy;
        });
    }
}
