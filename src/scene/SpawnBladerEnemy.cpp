//
// Created by antho on 2026-03-31.
//

#include "SpawnBladerEnemy.h"

#include <cmath>

#include "manager/AssetManager.h"

void SpawnBladerEnemy::spawn(World &world) {
    for (auto& beakEnemySpawnPoint : world.getMap().bladerEnemySpawnPoints) {
        auto& bladerSpawner(world.createEntity());
        auto& spawnerTransform = bladerSpawner.addComponent<Transform>
        (Vector2D(beakEnemySpawnPoint.x, beakEnemySpawnPoint.y), 0.0f, 1.0f);

        bladerSpawner.addComponent<SpawnOnVisible>(false, [&world](Transform spawnerTransform) {
            auto& bladerEnemy(world.createDeferredEntity());

            auto& bladerTransform = bladerEnemy.addComponent<Transform>
            (Vector2D(spawnerTransform.position.x, spawnerTransform.position.y), 0.0f, 1.0f);
            bladerEnemy.addComponent<Velocity>(Vector2D{-1, 1}, 200.0f, 0.0f);

            bladerEnemy.addComponent<MoveTowardsPlayer>(true, true, false, 200.0f);

            Animation anim = AssetManager::getAnimation("blader");
            bladerEnemy.addComponent<Animation>(anim);

            SDL_Texture* bladerTex = TextureManager::load("../Assets/Animations/blader_anim.png");

            SDL_FRect bladerSrc = anim.clips[anim.currentClip].frameIndices[0];
            SDL_FRect bladerDst {bladerTransform.position.x, bladerTransform.position.y, 48, 60};

            bladerEnemy.addComponent<Sprite>(bladerTex, bladerSrc, bladerDst);

            auto& bladerCollider = bladerEnemy.addComponent<Collider>("Enemy");
            bladerCollider.rect.w = bladerDst.w;
            bladerCollider.rect.h = bladerDst.h;

            bladerEnemy.addComponent<Health>(1);
            bladerEnemy.addComponent<ContactDamage>(1);

            bladerEnemy.addComponent<SpawnedEnemyTag>();
            bladerEnemy.addComponent<DestroyOutOfViewTag>();

            bladerEnemy.addComponent<BladerAttack>(false, bladerTransform.position.y, 1.0f, 0.5f, 0.0f);

            auto& bladerPlayerDetection(world.createEntity());
            auto& bladerDetectionTransform = bladerPlayerDetection.addComponent<Transform>(Vector2D(0.0f, 0.0f), 0.0f, 1.0f);
            auto& bladerDetectionCollider = bladerPlayerDetection.addComponent<Collider>("EnemyDetect");
            bladerDetectionCollider.rect.w = bladerDst.w * 5;
            bladerDetectionCollider.rect.h = 2000.0f;

            bladerPlayerDetection.addComponent<FollowEntity>
            (bladerEnemy, (-bladerDetectionCollider.rect.w / 2) + (bladerCollider.rect.w / 2),
                -bladerDetectionCollider.rect.h / 2);

            bladerPlayerDetection.addComponent<OnPlayerDetectCallback>([](Entity* bladerPlayerDetection, Entity* player) {
                auto& bladerEnemy = bladerPlayerDetection->getComponent<FollowEntity>().followedEntity;
                auto& bladerAttack = bladerEnemy.getComponent<BladerAttack>();
                if (bladerAttack.isAttacking) return;

                bladerAttack.isAttacking = true;
                bladerAttack.firstPhase = true;
                bladerAttack.timer = bladerAttack.firstAttackDuration;

                auto& playerTransform = player->getComponent<Transform>();
                auto& playerCollider = player->getComponent<Collider>();
                auto& bladerTransform = bladerEnemy.getComponent<Transform>();
                auto& bladerDetectionCollider = bladerPlayerDetection->getComponent<Collider>();
                bladerAttack.yAcceleration = (2 * (playerTransform.position.y + playerCollider.yOffset - bladerTransform.position.y)
                / std::pow(bladerAttack.firstAttackDuration, 2));

                auto& bladerVelocity = bladerEnemy.getComponent<Velocity>();
                bladerVelocity.xSpeed = 0;
                bladerVelocity.ySpeed = 0;
                bladerVelocity.direction.y = 1;
                if (playerTransform.position.x < bladerTransform.position.x) {
                    bladerVelocity.direction.x = -1;
                }
                else {
                    bladerVelocity.direction.x = 1;
                }
                bladerAttack.xAcceleration = (2 * (playerTransform.position.x + playerCollider.xOffset - bladerTransform.position.x)
                    / std::pow(bladerAttack.firstAttackDuration, 2)) * bladerVelocity.direction.x;
                bladerAttack.maxXDistance = (bladerDetectionCollider.rect.w - std::abs(playerTransform.position.x + playerCollider.xOffset - bladerTransform.position.x)) * 2 * bladerVelocity.direction.x;

                bladerEnemy.getComponent<MoveTowardsPlayer>().isMovingTowards = false;
                std::cout << "Blader detected player" << std::endl;
            });

            Animation newDeathAnim = AssetManager::getAnimation("enemyDeath");

            bladerEnemy.addComponent<OnDeathCallback>([&world, newDeathAnim] (Entity* blader) {
                auto& bladerDeath (world.createDeferredEntity());
                auto& bladerTransform = blader->getComponent<Transform>();
                auto& deathTransform = bladerDeath.addComponent<Transform>
                (Vector2D(bladerTransform.position.x, bladerTransform.position.y), 0.0f, 1.0f);

                auto& deathAnim = bladerDeath.addComponent<Animation>(newDeathAnim);

                SDL_Texture* beakDeathTex = TextureManager::load("../Assets/Animations/enemy_death_anim.png");
                SDL_FRect beakSrc = deathAnim.clips[deathAnim.currentClip].frameIndices[0];
                SDL_FRect beakDst {deathTransform.position.x, deathTransform.position.y, 48, 48};

                bladerDeath.addComponent<Sprite>(beakDeathTex, beakSrc, beakDst);
                bladerDeath.addComponent<EnemyDeathTag>();
            });

            bladerEnemy.addComponent<BladerEnemyTag>();
            return &bladerEnemy;
        });
    }
}
