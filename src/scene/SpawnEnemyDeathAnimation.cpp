//
// Created by antho on 2026-04-28.
//

#include "SpawnEnemyDeathAnimation.h"

void SpawnEnemyDeathAnimation::spawn(World &world, Entity &entity) {
    auto& death (world.createDeferredEntity());
    auto& entityTransform = entity.getComponent<Transform>();
    auto& deathTransform = death.addComponent<Transform>
    (Vector2D(entityTransform.position.x, entityTransform.position.y), 0.0f, 1.0f);

    Animation newDeathAnim = AssetManager::getAnimation("enemyDeath");
    auto& deathAnim = death.addComponent<Animation>(newDeathAnim);

    SDL_Texture* enemyDeathTex = TextureManager::load("Assets/Animations/enemy_death_anim.png");
    SDL_FRect deathSrc = deathAnim.clips[deathAnim.currentClip].frameIndices[0];
    SDL_FRect deathDst {deathTransform.position.x, deathTransform.position.y, 48, 48};

    death.addComponent<Sprite>(enemyDeathTex, deathSrc, deathDst);
    death.addComponent<EnemyDeathTag>();
}
