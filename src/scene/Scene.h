//
// Created by antho on 2026-02-25.
//

#ifndef TUTORIAL1_SCENE_H
#define TUTORIAL1_SCENE_H
#include <string>
#include <SDL3/SDL_events.h>
#include "World.h"
#include "Vector2D.h"

#include "SceneType.h"


class Scene {
public:
    Scene(SceneType sceneType, const char* sceneName, const char* mapPath, int windowWidth, int windowHeight);

    void update(const float dt, const SDL_Event &e) {
        world.update(dt, e, type);
    }

    void fixedUpdate(const float dt, const SDL_Event &e) {
        world.fixedUpdate(dt, e, type);
    }

    void render() {
        world.render();
    }

    World world;
    const std::string& getName() const { return name;}

private:
    std::string name;
    SceneType type;
    void createProjectile(Vector2D pos, Vector2D dir, int speed);

    void initMainMenu(int windowWidth, int windowHeight);
    void initGameplay(const char* mapPath, int windowWidth, int windowHeight);


};

#endif //TUTORIAL1_SCENE_H