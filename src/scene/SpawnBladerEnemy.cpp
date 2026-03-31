//
// Created by antho on 2026-03-31.
//

#include "SpawnBladerEnemy.h"

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
            bladerEnemy.addComponent<Velocity>(Vector2D{-1, 0}, 200.0f, 0.0f);

            Animation anim = AssetManager::getAnimation("blader");
            bladerEnemy.addComponent<Animation>(anim);

            SDL_Texture* bladerTex = TextureManager::load("../Assets/Animations/blader_anim.png");

            SDL_FRect bladerSrc = anim.clips[anim.currentClip].frameIndices[0];
            SDL_FRect bladerDst {bladerTransform.position.x, bladerTransform.position.y, 48, 60};

            bladerEnemy.addComponent<Sprite>(bladerTex, bladerSrc, bladerDst);

            auto& beakCollider = bladerEnemy.addComponent<Collider>("Enemy");
            beakCollider.rect.w = bladerDst.w;
            beakCollider.rect.h = bladerDst.h;

            bladerEnemy.addComponent<Health>(1);
            bladerEnemy.addComponent<ContactDamage>(1);

            bladerEnemy.addComponent<SpawnedEnemyTag>();
            bladerEnemy.addComponent<DestroyOutOfViewTag>();

            auto& bladerPlayerDetection(world.createEntity());
            auto& bladerDetectionTransform = bladerPlayerDetection.addComponent<Transform>(Vector2D(0.0f, 0.0f), 0.0f, 1.0f);
            auto& bladerDetectionCollider = bladerPlayerDetection.addComponent<Collider>("EnemyDetect");
            bladerDetectionCollider.rect.w = bladerDst.w * 5;
            bladerDetectionCollider.rect.h = 1000.0f;

            bladerPlayerDetection.addComponent<FollowEntity>
            (bladerEnemy, -bladerDetectionCollider.rect.w / 2, -bladerDetectionCollider.rect.h / 2);

            bladerPlayerDetection.addComponent<OnPlayerDetectCallback>([](Entity* bladerPlayerDetection) {
                std::cout << "Enemy detected player" << std::endl;
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
