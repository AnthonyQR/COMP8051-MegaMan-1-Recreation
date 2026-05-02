//
// Created by antho on 2026-02-25.
//
#include "Scene.h"

#include "../manager/AssetManager.h"
#include "Game.h"
#include "SpawnBeakEnemy.h"
#include "SpawnBigEyeEnemy.h"
#include "SpawnBladerEnemy.h"
#include "SpawnFleaEnemy.h"
#include "SpawnFlyingShellEnemy.h"
#include "SpawnOctopusBatteryEnemy.h"
#include "SpawnPlayer.h"
#include "SpawnScrewBomberEnemy.h"
#include "SpawnSuperCutterEnemy.h"
#include "Visuals/AnimationSystems/VictoryItemAnimationSystem.h"

Scene::Scene (SceneType sceneType, const char* sceneName, const char* mapPath, const int windowWidth, const int windowHeight)
: name(sceneName), type(sceneType) {
    if (sceneType == SceneType::Menu) {
        initMainMenu(windowWidth, windowHeight);
        return;
    }
    initGameplay(mapPath, windowWidth, windowHeight);
}

void Scene::initMainMenu(int windowWidth, int windowHeight) {
    // Camera
    auto &cam = world.createEntity();
    cam.addComponent<Camera>();

    // Menu
    auto &menu(world.createEntity());
    auto menuTransform = menu.addComponent<Transform>(Vector2D(0,0), 0.0f, 1.0f);

    Animation anim = AssetManager::getAnimation("titleScreen");
    anim.getAnimationClip = TitleScreenAnimationSystem::getAnimationClip;
    menu.addComponent<Animation>(anim);

    SDL_Texture *text = TextureManager::load("Assets/Animations/megaman_title_screen_anim.png");
    SDL_FRect menuSrc{0,0,256,240};
    SDL_FRect menuDst{menuTransform.position.x,menuTransform.position.y,(float)windowWidth, (float)windowHeight};

    menu.addComponent<Sprite>(text, menuSrc, menuDst);

    menu.addComponent<GameStarting>(false);
    menu.addComponent<TitleScreenTag>();
}

void Scene::initGameplay(const char *mapPath, int windowWidth, int windowHeight) {
    // Load our map
    world.getMap().load(mapPath, TextureManager::load("Assets/CutManTileset.png"));

    // Spawn Colliders
    for (auto &collider : world.getMap().colliders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("Wall");
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;

        /*
        // To have a visual of the colliders
        SDL_Texture* tex = TextureManager::load("Assets/tileset.png");
        SDL_FRect colSrc{0,32,32,32};
        SDL_FRect colDst {c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        e.addComponent<Sprite>(tex, colSrc, colDst);
        */
    }

    for (auto&item : world.getMap().itemSpawnPoints) {
        auto& entity(world.createEntity());
        entity.addComponent<Transform>(Vector2D(item.x, item.y), 0.0f, 1.0f);

        Animation anim = AssetManager::getAnimation("victoryItem");
        anim.getAnimationClip = VictoryItemAnimationSystem::getAnimationClip;
        entity.addComponent<Animation>(anim);

        SDL_Texture* itemTex = TextureManager::load("Assets/Animations/victory_item_anim.png");
        SDL_FRect itemSrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect itemDst {0, 0, 48, 48};
        entity.addComponent<Sprite>(itemTex, itemSrc, itemDst);

        auto& collider = entity.addComponent<Collider>("Item");
        collider.rect.x = item.x;
        collider.rect.y = item.y;
        collider.rect.w = itemDst.w;
        collider.rect.h = itemDst.h;

        entity.addComponent<VictoryItemTag>();
    }

    // Spawn Ladders
    for (auto &ladder : world.getMap().ladders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(ladder.rect.x, ladder.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("Ladder");
        c.rect.x = ladder.rect.x;
        c.rect.y = ladder.rect.y;
        c.rect.w = ladder.rect.w;
        c.rect.h = ladder.rect.h;
    }

    // Spawn Camera Bounds
    for (auto &cameraBounds : world.getMap().cameraBounds) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(cameraBounds.rect.x, cameraBounds.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("Camera Bounds");
        c.rect.x = cameraBounds.rect.x;
        c.rect.y = cameraBounds.rect.y;
        c.rect.w = cameraBounds.rect.w;
        c.rect.h = cameraBounds.rect.h;
    }

    // Spawn Camera
    auto& cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 48.0f, world.getMap().height * 48.0f);

    // Spawn Death Colliders
    for (auto &collider : world.getMap().deathColliders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("Death");
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;
    }

    // Spawn checkpoint colliders
    for (auto &collider : world.getMap().checkPointBoundsColliders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("Checkpoint");
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;
    }

    // Spawn Health bar in specific order (background -> fill -> dividers)
    Transform healthBarTransform = Transform(Vector2D(windowWidth / 12.0f, windowHeight / 12.0f), 0.0f, 1.0f);
    SDL_Texture* healthBarTex = TextureManager::load("Assets/megaman_health_bar.png");
    auto& healthBarBackground (world.createEntity());
    healthBarBackground.addComponent<Transform>(healthBarTransform);

    SDL_FRect healthBarBackgroundSrc{0, 0, 8, 56};
    SDL_FRect healthBarDst{healthBarTransform.position.x, healthBarTransform.position.y, 8 * 3, 56 * 3};
    healthBarBackground.addComponent<Sprite>(healthBarTex, healthBarBackgroundSrc, healthBarDst, RenderLayer::UI);


    auto& healthBarFill(world.createEntity());
    auto& healthBarFillTransform = healthBarFill.addComponent<Transform>
    (Vector2D(windowWidth / 12.0f, windowHeight / 12.0f + ((28 - Game::gameState.playerHealth) * 6)), 0.0f, 1.0f);
    SDL_FRect healthBarFillSrc {8, 0, 8, 56};
    SDL_FRect healthBarFillDst{healthBarTransform.position.x, healthBarTransform.position.y,
        8 * 3, 56 * 3 - (float)((28 - Game::gameState.playerHealth) * 6)};
    healthBarFill.addComponent<Sprite>(healthBarTex, healthBarFillSrc, healthBarFillDst, RenderLayer::UI);
    healthBarFill.addComponent<HealthBarUpdate>([windowHeight](Entity* healthBar) {
        healthBar->getComponent<Transform>().position.y = windowHeight / 12.0f + ((28 - Game::gameState.playerHealth) * 6);
        healthBar->getComponent<Sprite>().dst.h = 56 * 3 - (float)((28 - Game::gameState.playerHealth) * 6);
    });

    auto& healthBarDivider(world.createEntity());
    healthBarDivider.addComponent<Transform>(healthBarTransform);
    SDL_FRect healthBarDividerSrc {16, 0, 8, 52};
    healthBarDivider.addComponent<Sprite>(healthBarTex, healthBarDividerSrc, healthBarDst, RenderLayer::UI);

    // More complex spawns
    SpawnPlayer::spawn(world);
    SpawnBeakEnemy::spawn(world);
    SpawnBladerEnemy::spawn(world);
    SpawnOctopusBatteryEnemy::spawn(world);
    SpawnSuperCutterEnemy::spawn(world);
    SpawnFlyingShellEnemy::spawn(world);
    SpawnFleaEnemy::spawn(world);
    SpawnBigEyeEnemy::spawn(world);
    SpawnScrewBomberEnemy::spawn(world);

    // Add Scene State
    auto &state(world.CreateSceneStateEntity());
    state.addComponent<SceneState>(Game::gameState.playerHealth, Game::gameState.playerMaxHealth,
        Game::gameState.lives, Game::gameState.currentCheckpoint, Game::gameState.isEnding);

    auto& pause(world.createEntity());
    pause.addComponent<GamePaused>(false);
}
