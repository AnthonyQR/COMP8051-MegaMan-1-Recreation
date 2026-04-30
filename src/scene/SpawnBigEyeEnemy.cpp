//
// Created by antho on 2026-04-30.
//

#include "SpawnBigEyeEnemy.h"

#include "SpawnEnemyDeathAnimation.h"
#include "manager/AssetManager.h"
#include "Visuals/AnimationSystems/BigEyeAnimationSystem.h"

void SpawnBigEyeEnemy::spawn(World &world) {
    for (auto& bigEyeSpawnPoint : world.getMap().bigEyeSpawnPoints) {
        auto& bigEyeSpawner(world.createEntity());
        auto& spawnerTransform =bigEyeSpawner.addComponent<Transform>
        (Vector2D(bigEyeSpawnPoint.x, bigEyeSpawnPoint.y), 0.0f, 1.0f);


        bigEyeSpawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
            auto& bigEyeEnemy(world.createDeferredEntity());

            auto& bigEyeTransform = bigEyeEnemy.addComponent<Transform>
            (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
            bigEyeEnemy.addComponent<Velocity>(Vector2D{0, 0}, 0.0f, 0.0f);

            bigEyeEnemy.addComponent<TrackPlayer>();

            Animation anim = AssetManager::getAnimation("bigEye");
            anim.getAnimationClip = BigEyeAnimationSystem::getAnimationClip;
            bigEyeEnemy.addComponent<Animation>(anim);

            SDL_Texture* bigEyeTex = TextureManager::load("Assets/Animations/big_eye_anim.png");

            SDL_FRect bigEyeSrc = anim.clips[anim.currentClip].frameIndices[0];
            SDL_FRect bigEyeDst {bigEyeTransform.position.x, bigEyeTransform.position.y, 32 * 3, 48 * 3};

            bigEyeEnemy.addComponent<Sprite>(bigEyeTex, bigEyeSrc, bigEyeDst);

            auto& bigEyeCollider = bigEyeEnemy.addComponent<Collider>("Enemy");
            bigEyeCollider.rect.w = bigEyeDst.w;
            bigEyeCollider.rect.h = bigEyeDst.h;

            bigEyeEnemy.addComponent<WallCollidable>(true);

            bigEyeEnemy.addComponent<Health>(20);
            bigEyeEnemy.addComponent<ContactDamage>(10);

            bigEyeEnemy.addComponent<IsGrounded>(true);
            bigEyeEnemy.addComponent<Gravity>(2400.0f, 2400.0f, true);

            bigEyeEnemy.addComponent<Jump>();
            bigEyeEnemy.addComponent<AutoJump>(0.5f, std::vector<Vector2D>
            {
                Vector2D(230.0f, 600.0f),
                Vector2D(150.0f, 850.0f)
            }, true, 0.0f
            );

            bigEyeEnemy.addComponent<IsFacingRight>(false);

            bigEyeEnemy.addComponent<OnBottomCollisionCallback>([](Entity* bigEye) {
                auto& velocity = bigEye->getComponent<Velocity>();
                velocity.xSpeed = 0;

                for (auto& child : bigEye->getComponent<Children>().children) {
                    if (child->hasComponent<Hurtbox>()) {
                        auto& collider = child->getComponent<Collider>();
                        auto& bigEyeDst = bigEye->getComponent<Sprite>().dst;

                        collider.rect.h = bigEyeDst.h - (8 * 3);
                        collider.yOffset = (8 * 3);
                    }
                }

                auto& trackPlayer = bigEye->getComponent<TrackPlayer>();
                auto& isFacingRight = bigEye->getComponent<IsFacingRight>().facingRight;
                if (trackPlayer.isPlayerToTheRight) {
                    isFacingRight = true;
                }
                else {
                    isFacingRight = false;
                }
            });

            bigEyeEnemy.addComponent<OnJumpCallback>([](Entity* bigEye) {
                for (auto& child : bigEye->getComponent<Children>().children) {
                    if (child->hasComponent<Hurtbox>()) {
                        auto& collider = child->getComponent<Collider>();
                        auto& bigEyeDst = bigEye->getComponent<Sprite>().dst;

                        collider.rect.h = bigEyeDst.h;
                        collider.yOffset = 0;
                    }
                }

                auto& isFacingRight = bigEye->getComponent<IsFacingRight>().facingRight;
                auto& velocity = bigEye->getComponent<Velocity>();

                if (isFacingRight) {
                    velocity.xSpeed = std::abs(velocity.xSpeed);
                }
                else {
                    velocity.xSpeed = -std::abs(velocity.xSpeed);
                }
            });

            bigEyeEnemy.addComponent<SpawnedEnemyTag>();
            bigEyeEnemy.addComponent<DestroyOutOfViewTag>();

            auto& bigEyeHurtbox(world.createEntity());
            auto& bigEyeHurtboxCollider = bigEyeHurtbox.addComponent<Collider>("Enemy");
            bigEyeHurtboxCollider.rect.w = bigEyeCollider.rect.w;
            bigEyeHurtboxCollider.rect.h = bigEyeCollider.rect.h;
            bigEyeHurtbox.addComponent<Hurtbox>();
            bigEyeHurtbox.addComponent<Transform>(bigEyeTransform);
            bigEyeHurtbox.addComponent<Parent>(&bigEyeEnemy);
            bigEyeHurtbox.addComponent<FollowParent>();

            bigEyeEnemy.addComponent<OnDeathCallback>([&world] (Entity* bigEye) {
                SpawnEnemyDeathAnimation::spawn(world, *bigEye);
            });

            std::vector newChildren = {&bigEyeHurtbox};
            bigEyeEnemy.addComponent<Children>(newChildren);

            return &bigEyeEnemy;
        });
    }
};
