//
// Created by antho on 2026-04-28.
//

#include "SpawnFleaEnemy.h"

#include "SpawnEnemyDeathAnimation.h"
#include "manager/AssetManager.h"

void SpawnFleaEnemy::spawn(World &world) {
    for (auto& fleaSpawnPoint : world.getMap().fleaSpawnPoints) {
        auto& fleaSpawner(world.createEntity());
        auto& spawnerTransform =fleaSpawner.addComponent<Transform>
        (Vector2D(fleaSpawnPoint.x, fleaSpawnPoint.y), 0.0f, 1.0f);


        fleaSpawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
            auto& fleaEnemy(world.createDeferredEntity());

            auto& fleaTransform = fleaEnemy.addComponent<Transform>
            (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
            fleaEnemy.addComponent<Velocity>(Vector2D{0, 0}, 0.0f, 0.0f);

            fleaEnemy.addComponent<TrackPlayer>();

            Animation anim = AssetManager::getAnimation("flea");
            anim.getAnimationClip = FleaAnimationSystem::getAnimationClip;
            fleaEnemy.addComponent<Animation>(anim);

            SDL_Texture* fleaTex = TextureManager::load("Assets/Animations/flea_anim.png");

            SDL_FRect fleaSrc = anim.clips[anim.currentClip].frameIndices[0];
            SDL_FRect fleaDst {fleaTransform.position.x, fleaTransform.position.y, 42, 72};

            fleaEnemy.addComponent<Sprite>(fleaTex, fleaSrc, fleaDst);

            auto& fleaCollider = fleaEnemy.addComponent<Collider>("Enemy");
            fleaCollider.rect.w = fleaDst.w;
            fleaCollider.rect.h = fleaDst.h;

            fleaEnemy.addComponent<WallCollidable>(true);

            fleaEnemy.addComponent<Health>(1);
            fleaEnemy.addComponent<ContactDamage>(2);

            fleaEnemy.addComponent<IsGrounded>(true);
            fleaEnemy.addComponent<Gravity>(2400.0f, 2400.0f, true);

            fleaEnemy.addComponent<Jump>();
            fleaEnemy.addComponent<AutoJump>(0.5f, std::vector<Vector2D>
            {
                Vector2D(190.0f, 750.0f),
                Vector2D(480.0f, 700.0f)
            }, true, 0.0f
            );

            fleaEnemy.addComponent<OnBottomCollisionCallback>([](Entity* flea) {
                auto& velocity = flea->getComponent<Velocity>();
                velocity.xSpeed = 0;

                auto& collider = flea->getComponent<Collider>();
                auto& fleaDst = flea->getComponent<Sprite>().dst;

                collider.rect.h = fleaDst.h - (14 * 3);
                collider.yOffset = (14 * 3);
            });

            fleaEnemy.addComponent<OnJumpCallback>([](Entity* flea) {
                auto& collider = flea->getComponent<Collider>();
                auto& fleaDst = flea->getComponent<Sprite>().dst;

                collider.rect.h = fleaDst.h;
                collider.yOffset = 0;
            });

            fleaEnemy.addComponent<SpawnedEnemyTag>();
            fleaEnemy.addComponent<DestroyOutOfViewTag>();

            auto& fleaHurtbox(world.createEntity());
            auto& fleaHurtboxCollider = fleaHurtbox.addComponent<Collider>("Enemy");
            fleaHurtboxCollider.rect.w = fleaCollider.rect.w;
            fleaHurtboxCollider.rect.h = fleaCollider.rect.h;
            fleaHurtbox.addComponent<Hurtbox>();
            fleaHurtbox.addComponent<Transform>(fleaTransform);
            fleaHurtbox.addComponent<Parent>(&fleaEnemy);
            fleaHurtbox.addComponent<FollowParent>();

            fleaEnemy.addComponent<OnDeathCallback>([&world] (Entity* flea) {
                SpawnEnemyDeathAnimation::spawn(world, *flea);
            });

            std::vector newChildren = {&fleaHurtbox};
            fleaEnemy.addComponent<Children>(newChildren);

            fleaEnemy.addComponent<FleaEnemyTag>();

            return &fleaEnemy;
        });
    }
};
