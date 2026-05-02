//
// Created by antho on 2026-01-07.
//

#include "Game.h"
#include "Map.h"
// #include "GameObject.h"

#include <iostream>
#include <ostream>

#include "manager/AssetManager.h"

// GameObject *player = nullptr;

GameState Game::gameState;
std::function<void(std::string)> Game::onSceneChangeRequest;
std::function<void()> Game::checkSceneState;

Game::Game() {}

Game::~Game() {
    destroy();
}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Initialize SDL library
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 1) {
        std::cout << "Subsystem initialized..." << std::endl;
        window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
        if (window) {
            std::cout << "Window created..." << std::endl;
        }

        // Windows will be Direct3D (DirectX)
        // Mac will likely be Metal, OpenGL
        renderer = SDL_CreateRenderer(window, nullptr);


        if (renderer) {
            std::cout << "Renderer created..." << std::endl;
            SDL_SetRenderLogicalPresentation(renderer, 816, 672, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        }
        else {
            std::cout << "Renderer could not be created..." << std::endl;
            return;
        }

        isRunning = true;
    }
    else {
        isRunning = false;
    }

    // Check SDL Version
    int compiled = SDL_VERSION;
    const int linked = SDL_GetVersion();
    SDL_Log("We compiled against SDL version %d.%d.%d ...\n",
        SDL_VERSIONNUM_MAJOR(compiled),
        SDL_VERSIONNUM_MINOR(compiled),
        SDL_VERSIONNUM_MICRO(compiled));

    SDL_Log("But we are linking against SDL version %d.%d.%d.\n",
        SDL_VERSIONNUM_MAJOR(linked),
        SDL_VERSIONNUM_MINOR(linked),
        SDL_VERSIONNUM_MICRO(linked));

    // Load audio
    audioManager.loadAudio("gameStart", "Assets/Audio/GameStart.mp3", 10);
    audioManager.loadAudio("pause", "Assets/Audio/Pause.mp3", 20);

    audioManager.loadAudio("cutmanStage", "Assets/Audio/CutManStageLoop.mp3", 5);
    audioManager.loadAudio("victoryMusic", "Assets/Audio/VictoryMusic.mp3", 20);

    audioManager.loadAudio("megamanBuster", "Assets/Audio/MegaBuster.mp3", 7);
    audioManager.loadAudio("megamanDamage", "Assets/Audio/MegamanDamage.mp3", 10);
    audioManager.loadAudio("megamanDefeat", "Assets/Audio/MegamanDefeat.mp3", 10);
    audioManager.loadAudio("megamanLand", "Assets/Audio/MegamanLand.mp3", 1);

    audioManager.loadAudio("enemyShoot", "Assets/Audio/EnemyShoot.mp3", 6);
    audioManager.loadAudio("enemyDamage", "Assets/Audio/EnemyDamage.mp3", 8);
    audioManager.loadAudio("bigEyeLand", "Assets/Audio/BigEyeLand.mp3", 6);
    audioManager.loadAudio("dink", "Assets/Audio/Dink.mp3", 9);

    // Load assets
    AssetManager::loadAnimation("titleScreen", "Assets/Animations/megaman_title_screen_anim.xml");
    AssetManager::loadAnimation("player", "Assets/Animations/megaman_anim.xml");
    AssetManager::loadAnimation("playerHitParticles", "Assets/Animations/megaman_hit_particles_anim.xml");

    AssetManager::loadAnimation("beak", "Assets/Animations/beak_anim.xml");
    AssetManager::loadAnimation("blader", "Assets/Animations/blader_anim.xml");
    AssetManager::loadAnimation("octopus", "Assets/Animations/octopus_battery_anim.xml");
    AssetManager::loadAnimation("superCutter", "Assets/Animations/super_cutter_anim.xml");
    AssetManager::loadAnimation("flyingShell", "Assets/Animations/flying_shell_anim.xml");
    AssetManager::loadAnimation("enemyDeath", "Assets/Animations/enemy_death_anim.xml");
    AssetManager::loadAnimation("flea", "Assets/Animations/flea_anim.xml");
    AssetManager::loadAnimation("bigEye", "Assets/Animations/big_eye_anim.xml");
    AssetManager::loadAnimation("screwBomber", "Assets/Animations/screw_bomber_anim.xml");

    AssetManager::loadAnimation("victoryItem", "Assets/Animations/victory_item_anim.xml");
    AssetManager::loadAnimation("largeHealth", "Assets/Animations/large_health_anim.xml");

    // Load scenes
    sceneManager.loadScene(SceneType::Menu, "mainMenu", nullptr, width, height);
    sceneManager.loadScene(SceneType::Gameplay, "cutman", "Assets/CutManMap.tmx", width, height);
    sceneManager.loadScene(SceneType::Gameplay, "victory", "Assets/CutManMap.tmx", width, height);
    //sceneManager.loadScene(SceneType::Gameplay, "level1", "Assets/Map.tmx", width, height);
    //sceneManager.loadScene(SceneType::Gameplay, "level2", "Assets/map2.tmx", width, height);

    // Init game data / state
    gameState.playerHealth = 28;
    gameState.playerMaxHealth = 28;
    gameState.lives = 3;
    gameState.currentCheckpoint = 0;
    gameState.isEnding = false;
    gameState.isPaused = false;

    // Start Main Menu
    sceneManager.changeSceneDeferred("mainMenu");

    // Resolve scene callback
    onSceneChangeRequest = [this](std::string sceneName) {

        // Some game state happening here
        if (sceneManager.currentScene -> getName() == "level2" && sceneName == "level2") {
            std::cout << "You Win!" << std::endl;
            isRunning = false;
            return;
        }

        if (sceneName == "cutman") {
            audioManager.playMusic("cutmanStage");
        }

        if (sceneName == "cutman" && gameState.lives <= 0) {
            std::cout << "Game Over" << std::endl;
            isRunning = false;
            return;
        }

        if (sceneName == "victory") {
            std::cout << "You Win!" << std::endl;
            isRunning = false;
            return;
        }

        sceneManager.changeSceneDeferred(sceneName);
    };

    checkSceneState = [this]() {
        if (gameState.playerHealth <= 0) {
            gameState.playerHealth = gameState.playerMaxHealth;
            audioManager.stopMusic();
        }

        if (gameState.isEnding) {
            audioManager.stopMusic();
            audioManager.changeSfxVolume(0.15f);
        }
    };
}

void Game::handleEvents() {
    // SDL listens to the OS for input events internally, and it adds them to a queue

    // Check for next event, if an event is available, it will remove from the queue and store in event
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_EVENT_QUIT: // Usually triggered when the user closes the window
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update(float deltaTime) {
    frameCount++;
    sceneManager.update(deltaTime, event);
}

void Game::fixedUpdate(float deltaTime) {
    frameCount++;
    sceneManager.fixedUpdate(deltaTime, event);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Every frame, the renderer is cleared with the draw color
    SDL_RenderClear(renderer);

    sceneManager.render();

    // Swaps back buffer to the screen
    SDL_RenderPresent(renderer);
}

void Game::destroy() {
    TextureManager::clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game destroyed..." << std::endl;
}

// void RandomizePixels() {
//     if (frameCount % 60 != 0) {
//         return;
//     }
//
//     r = rand() % 256;
//     g = rand() % 256;
//     b = rand() % 256;
//     a = rand() % 256;
//
// }