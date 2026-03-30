//
// Created by antho on 2026-02-25.
//
#include "Scene.h"

#include "../manager/AssetManager.h"
#include "Game.h"
#include "SpawnBeakEnemy.h"
#include "SpawnPlayer.h"

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
    menu.addComponent<Animation>(anim);

    SDL_Texture *text = TextureManager::load("../Assets/Animations/megaman_title_screen_anim.png");
    SDL_FRect menuSrc{0,0,256,240};
    SDL_FRect menuDst{menuTransform.position.x,menuTransform.position.y,(float)windowWidth, (float)windowHeight};

    menu.addComponent<Sprite>(text, menuSrc, menuDst);

    menu.addComponent<GameStarting>(false);
    menu.addComponent<TitleScreenTag>();
}

void Scene::initGameplay(const char *mapPath, int windowWidth, int windowHeight) {
    // Load our map
    world.getMap().load(mapPath, TextureManager::load("../Assets/CutManTileset.png"));

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
        SDL_Texture* tex = TextureManager::load("../Assets/tileset.png");
        SDL_FRect colSrc{0,32,32,32};
        SDL_FRect colDst {c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        e.addComponent<Sprite>(tex, colSrc, colDst);
        */
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

    // Spawn items
    for (auto &itemSpawnPoint : world.getMap().itemSpawnPoints) {
        auto& item = world.createEntity();
        auto& itemTransform = item.addComponent<Transform>(Vector2D(itemSpawnPoint.x, itemSpawnPoint.y), 0.0f, 1.0f);

        SDL_Texture* itemTex = TextureManager::load("../Assets/coin.png");
        SDL_FRect itemSrc{0, 0, 32, 32};

        SDL_FRect itemDest{itemTransform.position.x, itemTransform.position.y, 32, 32};
        item.addComponent<Sprite>(itemTex, itemSrc, itemDest);

        auto& itemCollider = item.addComponent<Collider>("Item");
        itemCollider.rect.w = itemDest.w;
        itemCollider.rect.h = itemDest.h;
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

    // More complex spawns
    SpawnPlayer::spawn(world);
    SpawnBeakEnemy::spawn(world);

    // Add Scene State
    auto &state(world.createEntity());
    state.addComponent<SceneState>();
}
