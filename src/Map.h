//
// Created by antho on 2026-01-14.
//

#ifndef TUTORIAL1_MAP_H
#define TUTORIAL1_MAP_H

#include <vector>
#include <SDL3/SDL.h>
#include <Component.h>

#include "tinyxml2.h"

class Map {
public:
    Map() = default;
    ~Map() = default;

    void load(const char *path, SDL_Texture *ts);
    template <typename T>
    void parseData(tinyxml2::XMLElement* firstElement, std::string layerName, std::vector<T>& genericVector);
    void finishParse(tinyxml2::XMLElement* element, std::vector<Collider>& colliderList);
    void finishParse(tinyxml2::XMLElement* element, std::vector<Vector2D>& vector2DList);
    void draw(const Camera &cam);

    SDL_Texture *tileset = nullptr;
    int width{}, height{};
    std::vector<std::vector<int>> tileData;
    std::vector<Collider> colliders;
    std::vector<Collider> ladders;
    std::vector<Collider> cameraBounds;
    std::vector<Collider> deathColliders;
    std::vector<Vector2D> checkPointSpawnPoints;
    std::vector<Collider> checkPointBoundsColliders;
    std::vector<Vector2D> debugSpawnPoints;

    std::vector<Vector2D> beakEnemyRightSpawnPoints;
    std::vector<Vector2D> beakEnemyRightEarlySpawnPoints;
    std::vector<Vector2D> beakEnemyLeftSpawnPoints;
    std::vector<Vector2D> bladerEnemySpawnPoints;
    std::vector<Vector2D> octopusBatteryVerticalSpawnPoints;
    std::vector<Vector2D> octopusBatteryHorizontalSpawnPoints;
    std::vector<Vector2D> superCutterSpawnPoints;
    std::vector<Vector2D> flyingShellSpawnPoints;
    std::vector<Vector2D> flyingShellTimedSpawnPoints;
    std::vector<Vector2D> fleaSpawnPoints;
    std::vector<Vector2D> bigEyeSpawnPoints;
    std::vector<Vector2D> screwBomberTopSpawnPoints;
    std::vector<Vector2D> screwBomberBottomSpawnPoints;

    std::vector<Vector2D> itemSpawnPoints;
};

#endif //TUTORIAL1_MAP_H
