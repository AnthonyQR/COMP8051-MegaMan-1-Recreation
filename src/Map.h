//
// Created by antho on 2026-01-14.
//

#ifndef TUTORIAL1_MAP_H
#define TUTORIAL1_MAP_H

#include <vector>
#include <SDL3/SDL.h>
#include <Component.h>

class Map {
public:
    Map() = default;
    ~Map() = default;

    void load(const char *path, SDL_Texture *ts);
    void draw(const Camera &cam);

    SDL_Texture *tileset = nullptr;
    int width{}, height{};
    std::vector<std::vector<int>> tileData;
    std::vector<Collider> colliders;
    std::vector<Collider> ladders;
    std::vector<Collider> cameraBounds;
    std::vector<Vector2D> beakEnemyRightSpawnPoints;
    std::vector<Vector2D> beakEnemyRightEarlySpawnPoints;
    std::vector<Vector2D> beakEnemyLeftSpawnPoints;
    std::vector<Vector2D> bladerEnemySpawnPoints;
    std::vector<Collider> deathColliders;
    std::vector<Vector2D> itemSpawnPoints;
};

#endif //TUTORIAL1_MAP_H