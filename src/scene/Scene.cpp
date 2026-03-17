//
// Created by antho on 2026-02-25.
//
#include "Scene.h"

#include "../manager/AssetManager.h"
#include "Game.h"

Scene::Scene (SceneType sceneType, const char* sceneName, const char* mapPath, const int windowWidth, const int windowHeight)
: name(sceneName), type(sceneType) {
    if (sceneType == SceneType::Menu) {
        // Camera
        auto &cam = world.createEntity();
        cam.addComponent<Camera>();

        // Menu
        auto &menu(world.createEntity());
        auto menuTransform = menu.addComponent<Transform>(Vector2D(0,0), 0.0f, 1.0f);

        SDL_Texture *text = TextureManager::load("../Assets/MegamanTitleScreen.png");
        SDL_FRect menuSrc{0,0,256,240};
        SDL_FRect menuDst{menuTransform.position.x,menuTransform.position.y,(float)windowWidth, (float)windowHeight};

        menu.addComponent<Sprite>(text, menuSrc, menuDst);
        return;
    }

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

    auto& cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 48.0f, world.getMap().height * 48.0f);

    // Spawn Player
    auto& player(world.createEntity());
    auto& playerTransform = player.addComponent<Transform>(Vector2D(50, 6300), 0.0f, 1.0f);
    player.addComponent<Velocity>(Vector2D(0.0f,1.0f), 200.0f, 0.0f);
    player.addComponent<Gravity>(1800.0f, 1800.0f, true);
    player.addComponent<Jump>(660.0f, 7200.0f);

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture* tex = TextureManager::load("../Assets/Animations/fox_anim.png");
    // SDL_FRect playerSrc {0, 0, 32, 44};
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst {playerTransform.position.x, playerTransform.position.y, 48, 48};

    player.addComponent<Sprite>(tex, playerSrc, playerDst);

    auto& playerCollider = player.addComponent<Collider>("Player");
    playerCollider.rect.w = 42;
    playerCollider.rect.h = 48;

    player.addComponent<PlayerTag>();
    player.addComponent<Health>(Game::gameState.playerHealth);

    player.addComponent<IsGrounded>(false);

    player.addComponent<LadderClimbing>(200.0f);
    player.addComponent<KeyboardInputs>();

    // Spawn Spawner
    auto& spawner(world.createEntity());
    Transform t = spawner.addComponent<Transform>(Vector2D(windowWidth / 2, windowHeight - 5), 0.0f, 1.0f);
    spawner.addComponent<TimedSpawner>(2.0f, [this, t] {
        // Create our projectile
        auto& e(world.createDeferredEntity());
        e.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
        e.addComponent<Velocity>(Vector2D(0,-1), 100.0f);

        Animation anim = AssetManager::getAnimation("enemy");
        e.addComponent<Animation>(anim);

        SDL_Texture* tex = TextureManager::load("../Assets/Animations/bird_anim.png");
        SDL_FRect src {0, 0, 32, 32};
        SDL_FRect dest {t.position.x, t.position.y, 32, 32};
        e.addComponent<Sprite>(tex, src, dest);

        Collider c = e.addComponent<Collider>("Projectile");
        c.rect.w = dest.w;
        c.rect.h = dest.h;

        e.addComponent<ProjectileTag>();
    });

    // Add Scene State
    auto &state(world.createEntity());
    state.addComponent<SceneState>();
}
