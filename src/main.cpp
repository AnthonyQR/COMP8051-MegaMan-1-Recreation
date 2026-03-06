#include <iostream>

#include "Game.h"
#include "Vector2DTest.h"

// global variable
Game *game = nullptr;


int main() {
    const int FPS = 60; // 60 is the closest refresh rate of most of our monitors, 30fps is half the work
    const int desiredFrameTime = 1000 / FPS; // 16ms per frame

    Uint64 ticks = SDL_GetTicks();
    float deltaTime = 0.0f;
    int actualFrameTime;

    game = new Game();
    game->init("Tutorial Engine", 800, 600, false);

    // Vector2DTest *vector2DTest = new Vector2DTest();
    // vector2DTest -> Test();


    // Game loop
    while (game->running()) {
        int currentTicks = SDL_GetTicks(); // Time in ms since we initialized SDL
        deltaTime = (currentTicks - ticks) / 1000.0f;
        ticks = currentTicks;

        game->handleEvents();
        game->update(deltaTime);
        game->render();

        actualFrameTime = SDL_GetTicks() - ticks; // Elapsed time in ms it took the current frame

        // Frame limiter
        // Keeps the game running at the desired frame rate
        // If the actual frame took less time than the desired frame, delay the difference
        if (desiredFrameTime > actualFrameTime) {
            SDL_Delay(desiredFrameTime - actualFrameTime);
        }
    }

    delete game;

    return 0;
}

std::function<void(const CollisionEvent &)> Test() {
    Vector2D a(3, 4);
    Vector2D b(1,2);

    a += b;
    std::cout << "a + b = " << a.x << ", " << a.y << std::endl;

    Vector2D c = a - b;
    std:: cout << "a - b = " << c.x << ", " << c.y << std::endl;

    Vector2D d = a * 2;
    std::cout << "a * 2 = " << d.x << ", " << d.y << std::endl;

    Vector2D e = 2 * a;
    std::cout << "2 * a = " << e.x << ", " << e.y << std::endl;

    Vector2D f = -a;
    std::cout << "-a = " << f.x << ", " << f.y << std::endl;

    bool same = (a == b);
    std::cout << "a == b -> " << same << std::endl;
}