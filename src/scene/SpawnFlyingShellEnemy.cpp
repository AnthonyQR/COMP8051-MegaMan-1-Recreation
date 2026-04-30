//
//
// Created by antho on 2026-04-07.
//

#include "SpawnFlyingShellEnemy.h"

#include "SpawnEnemyDeathAnimation.h"
#include "Vector2D.h"
#include "World.h"
#include "manager/AssetManager.h"

void SpawnFlyingShellEnemy::spawn(World &world) {
    for (auto& shellEnemySpawnPoint : world.getMap().flyingShellSpawnPoints) {
        auto& shellSpawner(world.createEntity());
        auto& spawnerTransform = shellSpawner.addComponent<Transform>
        (Vector2D(shellEnemySpawnPoint.x, shellEnemySpawnPoint.y), 0.0f, 1.0f);

        shellSpawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
            auto shellEnemy = finishSpawn(world, spawnerTransform);
            return shellEnemy;
        });
    }
    for (auto& shellEnemySpawnPoint : world.getMap().flyingShellTimedSpawnPoints) {
        auto& shellSpawner(world.createEntity());
        auto& spawnerTransform = shellSpawner.addComponent<Transform>
        (Vector2D(shellEnemySpawnPoint.x, shellEnemySpawnPoint.y), 0.0f, 1.0f);

        shellSpawner.addComponent<SpawnWhileVisibleTimer>(1.0f, 5.0f, [&world](Transform spawnerTransform) {
            auto shellEnemy = finishSpawn(world, spawnerTransform);
            return shellEnemy;
        });
    }
}

Entity * SpawnFlyingShellEnemy::finishSpawn(World &world, Transform spawnerTransform) {
    auto& shellEnemy(world.createDeferredEntity());
    auto& shellTransform = shellEnemy.addComponent<Transform>
    (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
    shellEnemy.addComponent<Velocity>(Vector2D{-1, 0}, 200.0f, 0.0f);
    shellEnemy.addComponent<IsFacingRight>(false);
    shellEnemy.addComponent<StopMovementWhileFiring>(Vector2D{-1, 0});

    Animation anim = AssetManager::getAnimation("flyingShell");
    anim.getAnimationClip = FlyingShellAnimationSystem::getAnimationClip;
    shellEnemy.addComponent<Animation>(anim);

    SDL_Texture* shellTex = TextureManager::load("Assets/Animations/flying_shell_anim.png");

    SDL_FRect shellSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect shellDst {shellTransform.position.x, shellTransform.position.y, 51, 63};

    shellEnemy.addComponent<Sprite>(shellTex, shellSrc, shellDst);

    auto& shellCollider = shellEnemy.addComponent<Collider>("Enemy");
    shellCollider.rect.w = shellDst.w;
    shellCollider.rect.h = shellDst.h;

    SDL_Texture* shellProjectileTex = TextureManager::load("Assets/flying_shell_projectile.png");
    SDL_FRect shellProjectileSrc{0, 0, 6, 6};
    SDL_FRect shellProjectileDest{0, 0, 6 * 3, 6 * 3};
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

    auto& shellHurtbox(world.createEntity());
    auto& shellHurtboxCollider = shellHurtbox.addComponent<Collider>("Enemy");
    shellHurtboxCollider.rect.w = shellCollider.rect.w;
    shellHurtboxCollider.rect.h = shellCollider.rect.h;
    shellHurtbox.addComponent<Hurtbox>();
    shellHurtbox.addComponent<Transform>(shellTransform);
    shellHurtbox.addComponent<Parent>(&shellEnemy);
    shellHurtbox.addComponent<FollowParent>();

    shellEnemy.addComponent<OnDeathCallback>([&world](Entity* shell) {
        SpawnEnemyDeathAnimation::spawn(world, *shell);
    });

    std::vector newChildren = {&shellHurtbox};
    shellEnemy.addComponent<Children>(newChildren);

    return &shellEnemy;
}
