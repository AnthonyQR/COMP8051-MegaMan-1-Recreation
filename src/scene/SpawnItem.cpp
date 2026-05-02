//
// Created by antho on 2026-05-01.
//

#include "SpawnItem.h"

#include "manager/AssetManager.h"
#include "Visuals/AnimationSystems/LargeHealthAnimationSystem.h"

void SpawnItem::spawn(World& world, Entity& entity) {
    int totalDropRolls = 128;
    Vector2D oneUpChance = {0, 1}; // 1 / 128
    Vector2D smallHealthChance = {2, 16}; // 15 / 128
    Vector2D largeHealingChance = {17, 19}; // 2 / 128

    int random_int = rand() % totalDropRolls;
    if (random_int >= oneUpChance.x &&
        random_int <= oneUpChance.y) {
        spawnOneUp(world, entity);
    }
    else if (random_int >= smallHealthChance.x &&
             random_int <= smallHealthChance.y) {
        spawnSmallHealth(world, entity);
    }
    else if (random_int >= largeHealingChance.x &&
             random_int <= largeHealingChance.y) {
        spawnLargeHealth(world, entity);
    }
    // Spawn nothing otherwise
}

void SpawnItem::spawnSmallHealth(World &world, Entity &entity) {
    auto& smallHealth (world.createDeferredEntity());

    auto& entityTransform = entity.getComponent<Transform>();
    auto& smallHealthTransform = smallHealth.addComponent<Transform>
    (Vector2D(entityTransform.position.x, entityTransform.position.y), 0.0f, 1.0f);
    smallHealth.addComponent<Velocity>(Vector2D{0, 0}, 0.0f, 0.0f);

    SDL_Texture* smallHealthTex = TextureManager::load("Assets/small_health.png");

    SDL_FRect smallHealthSrc = {0, 0, 10, 12};
    SDL_FRect smallHealthDst {smallHealthTransform.position.x, smallHealthTransform.position.y, 30, 36};

    smallHealth.addComponent<Sprite>(smallHealthTex, smallHealthSrc, smallHealthDst);

    auto& smallHealthCollider = smallHealth.addComponent<Collider>("Item");
    smallHealthCollider.rect.w = smallHealthDst.w;
    smallHealthCollider.rect.h = smallHealthDst.h;

    smallHealth.addComponent<WallCollidable>(true);

    smallHealth.addComponent<Gravity>(2400.0f, 2400.0f, true);

    smallHealth.addComponent<Healing>(2);

    smallHealth.addComponent<DestroyOutOfViewTag>();
}

void SpawnItem::spawnLargeHealth(World &world, Entity &entity) {
    auto& largeHealth (world.createDeferredEntity());

    auto& entityTransform = entity.getComponent<Transform>();
    auto& largeHealthTransform = largeHealth.addComponent<Transform>
    (Vector2D(entityTransform.position.x, entityTransform.position.y), 0.0f, 1.0f);
    largeHealth.addComponent<Velocity>(Vector2D{0, 0}, 0.0f, 0.0f);

    Animation anim = AssetManager::getAnimation("largeHealth");
    anim.getAnimationClip = LargeHealthAnimationSystem::getAnimationClip;
    largeHealth.addComponent<Animation>(anim);

    SDL_Texture* largeHealthTex = TextureManager::load("Assets/Animations/large_health_anim.png");

    SDL_FRect largeHealthSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect largeHealthDst {largeHealthTransform.position.x, largeHealthTransform.position.y, 48, 48};

    largeHealth.addComponent<Sprite>(largeHealthTex, largeHealthSrc, largeHealthDst);

    auto& largeHealthCollider = largeHealth.addComponent<Collider>("Item");
    largeHealthCollider.rect.w = largeHealthDst.w;
    largeHealthCollider.rect.h = largeHealthDst.h;

    largeHealth.addComponent<WallCollidable>(true);

    largeHealth.addComponent<Gravity>(2400.0f, 2400.0f, true);

    largeHealth.addComponent<Healing>(10);

    largeHealth.addComponent<DestroyOutOfViewTag>();
}

void SpawnItem::spawnOneUp(World &world, Entity &entity) {
    auto& oneUp (world.createDeferredEntity());

    auto& entityTransform = entity.getComponent<Transform>();
    auto& oneUpTransform = oneUp.addComponent<Transform>
    (Vector2D(entityTransform.position.x, entityTransform.position.y), 0.0f, 1.0f);
    oneUp.addComponent<Velocity>(Vector2D{0, 0}, 0.0f, 0.0f);

    SDL_Texture* oneUpTex = TextureManager::load("Assets/one_up.png");

    SDL_FRect oneUpSrc = {0, 0, 16, 16};
    SDL_FRect oneUpDst {oneUpTransform.position.x, oneUpTransform.position.y, 48, 48};

    oneUp.addComponent<Sprite>(oneUpTex, oneUpSrc, oneUpDst);

    auto& oneUpCollider = oneUp.addComponent<Collider>("Item");
    oneUpCollider.rect.w = oneUpDst.w;
    oneUpCollider.rect.h = oneUpDst.h;

    oneUp.addComponent<WallCollidable>(true);

    oneUp.addComponent<Gravity>(2400.0f, 2400.0f, true);

    oneUp.addComponent<OneUp>();

    oneUp.addComponent<DestroyOutOfViewTag>();
}

