//
// Created by antho on 2026-01-14.
//

#include "Map.h"

#include <cmath>
#include <iostream>

#include "TextureManager.h"
#include <sstream>
#include <tinyxml2.h>

void Map::load(const char* path, SDL_Texture *ts) {
    tileset = ts;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    // Parse width and height of map
    auto* mapNode = doc.FirstChildElement("map");
    width = mapNode->IntAttribute("width");
    height = mapNode->IntAttribute("height");

    // Parse terrain data
    auto* layer = mapNode->FirstChildElement("layer");
    auto* data = layer->FirstChildElement("data");
    std::string csv = data->GetText();
    std::stringstream ss(csv);
    tileData = std::vector(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::string val;

            // Read characters from a ss into val until it hits a comma, or is at the end of the stream
            if (!std::getline(ss, val, ',')) break;
            tileData[i][j] = std::stoi(val); // stoi is a string to integer converter
        }
    }

    // Parse collider data
    parseData(layer, "Collision Layer", colliders);
    parseData(layer, "Ladder Layer", ladders);
    parseData(layer, "Camera Bounds Layer", cameraBounds);
    parseData(layer, "Death Collision Layer", deathColliders);
    parseData(layer, "Checkpoint Spawn Layer", checkPointSpawnPoints);
    parseData(layer, "Checkpoint Collision Layer", checkPointBoundsColliders);
    parseData(layer, "Debug Spawn Points Layer", debugSpawnPoints);

    parseData(layer, "Beak Left Spawn Layer", beakEnemyLeftSpawnPoints);
    parseData(layer, "Beak Right Spawn Layer", beakEnemyRightSpawnPoints);
    parseData(layer, "Beak Right Early Spawn Layer", beakEnemyRightEarlySpawnPoints);
    parseData(layer, "Blader Enemy Spawn Layer", bladerEnemySpawnPoints);
    parseData(layer, "Octopus Battery Vertical Spawn Layer", octopusBatteryVerticalSpawnPoints);
    parseData(layer, "Octopus Battery Horizontal Spawn Layer", octopusBatteryHorizontalSpawnPoints);
    parseData(layer, "Super Cutter Spawn Layer", superCutterSpawnPoints);
    parseData(layer, "Flying Shell Spawn Layer", flyingShellSpawnPoints);
    parseData(layer, "Flying Shell Timed Spawn Layer", flyingShellTimedSpawnPoints);

    parseData(layer, "Item Spawn Points Layer", itemSpawnPoints);
}

template<typename T>
void Map::parseData(tinyxml2::XMLElement *firstElement, std::string layerName, std::vector<T>& genericVector) {
    auto* objectGroup = firstElement->NextSiblingElement("objectgroup"); // Set to the first object group

    if (!objectGroup) return;

    // If the name is wrong, iterate through the object groups until the element with the same name is found
    if (objectGroup->Attribute("name") != layerName) {
        for (auto* objGroup = objectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == layerName) {
                objectGroup = objGroup;
                break;
            }
        }
    }
    // Parse data if the correct object group was found
    if (objectGroup->Attribute("name") == layerName) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = objectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment
            finishParse(obj, genericVector);
        }
    }
}

void Map::finishParse(tinyxml2::XMLElement *element, std::vector<Collider>& colliderList) {
    Collider c;
    c.rect.x = element->FloatAttribute("x") * 3;
    c.rect.y = element->FloatAttribute("y") * 3;
    c.rect.w = element->FloatAttribute("width") * 3;
    c.rect.h = element->FloatAttribute("height") * 3;
    colliderList.push_back(c);
}

void Map::finishParse(tinyxml2::XMLElement *element, std::vector<Vector2D>& vector2DList) {
    Vector2D pos;
    pos.x = element->FloatAttribute("x") * 3;
    pos.y = element->FloatAttribute("y") * 3;
    vector2DList.push_back(pos);
}

void Map::draw(const Camera &cam) {
    SDL_FRect src{}, dest{};

    dest.w = dest.h = 48;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int type = tileData[row][col];

            float worldX = static_cast<float>(col) * dest.w;
            float worldY = static_cast<float>(row) * dest.h;

            // Move the tiles or map relative to the camera
            // Convert from world space to screen space
            dest.x = std::round(worldX - cam.view.x);
            dest.y = std::round(worldY - cam.view.y);

            // All tiles are the same width & height
            // Calculate x & y with modulus
            type -= 1;
            src.x = (type % 6) * 16;
            src.y = ((type / 6) % 6) * 16;
            src.w = 16;
            src.h = 16;

            TextureManager::draw(tileset, src, dest);
        }
    }
}
