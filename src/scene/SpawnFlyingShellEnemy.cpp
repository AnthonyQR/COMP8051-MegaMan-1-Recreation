//
// Created by antho on 2026-04-07.
//

#include "SpawnFlyingShellEnemy.h"

#include "Vector2D.h"
#include "World.h"
#include "manager/AssetManager.h"

void SpawnFlyingShellEnemy::spawn(World &world) {
    for (auto& shellEnemySpawnPoint : world.getMap().flyingShellSpawnPoints) {
        auto& shellSpawner(world.createEntity());
        auto& spawnerTransform = shellSpawner.addComponent<Transform>
        (Vector2D(shellEnemySpawnPoint.x, shellEnemySpawnPoint.y), 0.0f, 1.0f);

        shellSpawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
            auto& shellEnemy(world.createDeferredEntity());
            auto& shellTransform = shellEnemy.addComponent<Transform>
            (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
            shellEnemy.addComponent<Velocity>(Vector2D{-1, 0}, 200.0f, 0.0f);
            shellEnemy.addComponent<IsFacingRight>(false);
            shellEnemy.addComponent<StopMovementWhileFiring>(Vector2D{-1, 0});

            Animation anim = AssetManager::getAnimation("flyingShell");
            shellEnemy.addComponent<Animation>(anim);

            SDL_Texture* shellTex = TextureManager::load("../Assets/Animations/flying_shell_anim.png");

            SDL_FRect shellSrc = anim.clips[anim.currentClip].frameIndices[0];
            SDL_FRect shellDst {shellTransform.position.x, shellTransform.position.y, 51, 63};

            shellEnemy.addComponent<Sprite>(shellTex, shellSrc, shellDst);

            auto& shellCollider = shellEnemy.addComponent<Collider>("Enemy");
            shellCollider.rect.w = shellDst.w;
            shellCollider.rect.h = shellDst.h;

            SDL_Texture* shellProjectileTex = TextureManager::load("../Assets/flying_shell_projectile.png");
            SDL_FRect shellProjectileSrc{0, 0, 8, 8};
            SDL_FRect shellProjectileDest{0, 0, 8 * 3, 8 * 3};
            Sprite newProjectileSprite{shellProjectileTex, shellProjectileSrc, shellProjectileDest};

            shellEnemy.addComponent<ProjectileStats>(800.0f, 2, Vector2D(0, 0), Vector2D(shellTransform.position.x, shellTransform.position.y),
            [&world, newProjectileSprite](ProjectileStats stats, Entity entity) {
                auto& shellTransform = entity.getComponent<Transform>();
                auto& projectile = world.createDeferredEntity();
                auto& projectileTransform = projectile.addComponent<Transform>
                (Vector2D(shellTransform.position.x, shellTransform.position.y) ,0.0f, 1.0f);
                auto& projectileSprite = projectile.addComponent<Sprite>(newProjectileSprite);
                auto& projectileCollider = projectile.addComponent<Collider>("Projectile");
                projectileCollider.rect.w = projectileSprite.dst.w;
                projectileCollider.rect.h = projectileSprite.dst.h;
                projectile.addComponent<ProjectileTag>();
                auto& projectileVelocity = projectile.addComponent<Velocity>(stats.direction, stats.projectileSpeed, stats.projectileSpeed);
                projectile.addComponent<ProjectileDamage>(stats.damage);
                projectile.addComponent<DestroyOutOfViewTag>();
                world.getAudioEventQueue().push(std::make_unique<AudioEvent>("enemyShoot"));
            });

            shellEnemy.addComponent<IsFiring>(false, 0.15f, 0.15f, 0.15f);
            shellEnemy.addComponent<FlyingShellEnemyTag>();
            shellEnemy.addComponent<AutoFiring>(1.0f, std::vector<FiringPattern>
            {
                {Vector2D(-1.0f, 0.0f).normalize(), 0.0f},
                {Vector2D(-1.0f, 1.0f).normalize(), 0.0f},
                {Vector2D(0.0f, 1.0f).normalize(), 0.0f},
                {Vector2D(1.0f, 1.0f).normalize(), 0.0f},
                {Vector2D(1.0f, 0.0f).normalize(), 0.0f},
                {Vector2D(1.0f, -1.0f).normalize(), 0.0f},
                {Vector2D(0.0f, -1.0f).normalize(), 0.0f},
                {Vector2D(-1.0f, -1.0f).normalize(), 0.0f}
            }, true, true, 1.0f);

            shellEnemy.addComponent<Health>(1);
            shellEnemy.addComponent<ContactDamage>(1);
            shellEnemy.addComponent<Invulnerability>(false);
            shellEnemy.addComponent<InvulnerableWhileNotFiring>();

            shellEnemy.addComponent<SpawnedEnemyTag>();
            shellEnemy.addComponent<DestroyOutOfViewTag>();

            Animation newDeathAnim = AssetManager::getAnimation("enemyDeath");

            shellEnemy.addComponent<OnDeathCallback>([&world, newDeathAnim](Entity* shell) {
               auto& shellDeath (world.createDeferredEntity());
               auto& shellTransform = shell->getComponent<Transform>();
               auto& deathTransform = shellDeath.addComponent<Transform>
               (Vector2D(shellTransform.position.x, shellTransform.position.y), 0.0f, 1.0f);

               auto& deathAnim = shellDeath.addComponent<Animation>(newDeathAnim);

               SDL_Texture* shellDeathTex = TextureManager::load("../Assets/Animations/enemy_death_anim.png");
               SDL_FRect shellSrc = deathAnim.clips[deathAnim.currentClip].frameIndices[0];
               SDL_FRect shellDst {deathTransform.position.x, deathTransform.position.y, 48, 48};

               shellDeath.addComponent<Sprite>(shellDeathTex, shellSrc, shellDst);
               shellDeath.addComponent<EnemyDeathTag>();
            });

            return &shellEnemy;
        });
    }
}
