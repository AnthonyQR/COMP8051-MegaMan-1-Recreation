//
// Created by antho on 2026-03-31.
//

#include "SpawnBladerEnemy.h"

#include <cmath>

#include "SpawnEnemyDeathAnimation.h"
#include "manager/AssetManager.h"

void SpawnBladerEnemy::spawn(World &world) {
    for (auto& bladerEnemySpawnPoint : world.getMap().bladerEnemySpawnPoints) {
        auto& bladerSpawner(world.createEntity());
        auto& spawnerTransform = bladerSpawner.addComponent<Transform>
        (Vector2D(bladerEnemySpawnPoint.x, bladerEnemySpawnPoint.y), 0.0f, 1.0f);

        bladerSpawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
            auto& bladerEnemy(world.createDeferredEntity());

            auto& bladerTransform = bladerEnemy.addComponent<Transform>
            (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
            bladerEnemy.addComponent<Velocity>(Vector2D{-1, 1}, 200.0f, 0.0f);

            bladerEnemy.addComponent<TrackPlayer>();
            bladerEnemy.addComponent<MoveTowardsPlayer>(true, true, false, 200.0f);

            Animation anim = AssetManager::getAnimation("blader");
            anim.getAnimationClip = BladerAnimationSystem::getAnimationClip;
            bladerEnemy.addComponent<Animation>(anim);

            SDL_Texture* bladerTex = TextureManager::load("Assets/Animations/blader_anim.png");

            SDL_FRect bladerSrc = anim.clips[anim.currentClip].frameIndices[0];
            SDL_FRect bladerDst {bladerTransform.position.x, bladerTransform.position.y, 48, 60};

            bladerEnemy.addComponent<Sprite>(bladerTex, bladerSrc, bladerDst);

            auto& bladerCollider = bladerEnemy.addComponent<Collider>("Enemy");
            bladerCollider.rect.w = bladerDst.w;
            bladerCollider.rect.h = bladerDst.h;

            bladerEnemy.addComponent<Health>(1);
            bladerEnemy.addComponent<ContactDamage>(3);

            bladerEnemy.addComponent<SpawnedEnemyTag>();
            bladerEnemy.addComponent<DestroyOutOfViewTag>();

            bladerEnemy.addComponent<BladerAttack>(false, bladerTransform.position.y, 0.8f, 0.5f, 12.0f);

            auto& bladerPlayerDetection(world.createDeferredEntity());
            auto& bladerDetectionTransform = bladerPlayerDetection.addComponent<Transform>(Vector2D(0.0f, 0.0f), 0.0f, 1.0f);
            auto& bladerDetectionCollider = bladerPlayerDetection.addComponent<Collider>("EnemyDetect");
            bladerDetectionCollider.rect.w = bladerDst.w * 5;
            bladerDetectionCollider.rect.h = 2000.0f;

            bladerPlayerDetection.addComponent<Parent>(&bladerEnemy);
            bladerPlayerDetection.addComponent<FollowParent>
            ((-bladerDetectionCollider.rect.w / 2) + (bladerCollider.rect.w / 2),
                -bladerDetectionCollider.rect.h / 2);


            bladerPlayerDetection.addComponent<OnPlayerDetectStayCallback>([](Entity* bladerPlayerDetection, Entity* player) {
                auto& bladerEnemy = bladerPlayerDetection->getComponent<Parent>().parent;
                auto& bladerAttack = bladerEnemy->getComponent<BladerAttack>();
                if (bladerAttack.isAttacking) return;

                bladerAttack.isAttacking = true;
                bladerAttack.firstPhase = true;
                bladerAttack.timer = bladerAttack.firstAttackDuration;

                auto& playerTransform = player->getComponent<Transform>();
                auto& playerCollider = player->getComponent<Collider>();
                auto& bladerTransform = bladerEnemy->getComponent<Transform>();
                auto& bladerCollider = bladerEnemy->getComponent<Collider>();
                auto& bladerDetectionCollider = bladerPlayerDetection->getComponent<Collider>();

                auto& bladerVelocity = bladerEnemy->getComponent<Velocity>();
                bladerVelocity.xSpeed = 0;
                bladerVelocity.ySpeed = 0;

                bladerVelocity.direction.y = 1;
                if (playerTransform.position.x < bladerTransform.position.x) {
                    bladerVelocity.direction.x = -1;
                }
                else {
                    bladerVelocity.direction.x = 1;
                }

                bladerAttack.yAcceleration =
                    (2 * (playerTransform.position.y + playerCollider.yOffset - bladerTransform.position.y)
                    / std::pow(bladerAttack.firstAttackDuration, 2));

                if (bladerVelocity.direction.x == 1) {
                    bladerAttack.xAcceleration =
                    (2 * (playerTransform.position.x + (playerCollider.rect.w / 2) - (playerCollider.xOffset)
                        - bladerTransform.position.x - bladerAttack.firstPhaseDistanceFromPlayer)
                    / std::pow(bladerAttack.firstAttackDuration, 2)) * bladerVelocity.direction.x;

                    bladerAttack.secondPhaseXDistance =
                        (bladerDetectionCollider.rect.w / 1.25f) * bladerVelocity.direction.x;
                }
                else {
                    bladerAttack.xAcceleration =
                    (2 * (playerTransform.position.x + (playerCollider.rect.w / 2) - (playerCollider.xOffset)
                    + bladerAttack.firstPhaseDistanceFromPlayer - bladerTransform.position.x + bladerCollider.rect.w)
                    / std::pow(bladerAttack.firstAttackDuration, 2)) * bladerVelocity.direction.x;

                    bladerAttack.secondPhaseXDistance =
                        (bladerDetectionCollider.rect.w / 1.25f) * bladerVelocity.direction.x;
                    }


                bladerEnemy->getComponent<MoveTowardsPlayer>().isMovingTowards = false;
                std::cout << "Blader detected player" << std::endl;
            });

            auto& bladerHurtbox(world.createEntity());
            auto& bladerHurtboxCollider = bladerHurtbox.addComponent<Collider>("Enemy");
            bladerHurtboxCollider.rect.w = bladerCollider.rect.w;
            bladerHurtboxCollider.rect.h = bladerCollider.rect.h;
            bladerHurtbox.addComponent<Hurtbox>();
            bladerHurtbox.addComponent<Transform>(bladerTransform);
            bladerHurtbox.addComponent<Parent>(&bladerEnemy);
            bladerHurtbox.addComponent<FollowParent>();

            bladerEnemy.addComponent<OnDeathCallback>([&world] (Entity* blader) {
                SpawnEnemyDeathAnimation::spawn(world, *blader);
            });

            std::vector newChildren = {&bladerHurtbox, &bladerPlayerDetection};
            bladerEnemy.addComponent<Children>(newChildren);

            bladerEnemy.addComponent<BladerEnemyTag>();
            return &bladerEnemy;
        });
    }
}
