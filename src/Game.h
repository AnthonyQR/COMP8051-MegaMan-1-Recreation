//
// Created by antho on 2026-01-07.
//

// #ifndef TUTORIAL1_GAME_H
// #define TUTORIAL1_GAME_H
//
// #endif //TUTORIAL1_GAME_H

#pragma once
#include <SDL3/SDL.h>

#include "World.h"
#include "manager/AudioManager.h"
#include "manager/SceneManager.h"

struct GameState {
    int playerHealth;
    int playerMaxHealth;
    int lives;
    int currentCheckpoint;
};

class Game {
    public:
    Game();
    ~Game();

    // Types that SDL uses
    void init(const char* title, int width, int height, bool fullscreen);

    // Game loop functions (handle events, update, render)
    void handleEvents(); // Checks for input and system events
    void update(float deltaTime); // Handles the game logic and changes to game state
    void render(); // Handles the drawing of the current game state to the screen

    // Used to free resources
    void destroy();

    bool running() {
        return isRunning;
    }

    SDL_Renderer* renderer = nullptr;

    AudioManager audioManager;

    SceneManager sceneManager;
    static std::function<void(std::string)> onSceneChangeRequest;
    static std::function<void()> checkSceneState;
    static GameState gameState;

    World world;

private:
    int frameCount = 0;
    bool isRunning = false;

    SDL_Window* window = nullptr;
    SDL_Event event;

    // SDL stores color channels as 8-bit unsigned
    // Range is 0-255
    Uint8 r,g,b,a;
};
