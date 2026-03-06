//
// Created by antho on 2026-01-14.
//

#include "Map.h"

#include <cmath>

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
    auto* collidersObjectGroup = layer->NextSiblingElement("objectgroup");; // Set to the first object group

    // If the name is wrong, iterate through the object groups until the element with the same name is found
    if (collidersObjectGroup->Attribute("name") != std::string("Collision Layer")) {
        for (auto* objGroup = collidersObjectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Collision Layer")) {
                collidersObjectGroup = objGroup;
                break;
            }
        }
    }
    // Parse data if the correct object group was found
    if (collidersObjectGroup->Attribute("name") == std::string("Collision Layer")) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = collidersObjectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment

            Collider c;
            c.rect.x = obj->FloatAttribute("x");
            c.rect.y = obj->FloatAttribute("y");
            c.rect.w = obj->FloatAttribute("width");
            c.rect.h = obj->FloatAttribute("height");
            colliders.push_back(c);
            }
    }

    // Parse item spawn points
    auto* itemSpawnPointsGroup = layer->NextSiblingElement("objectgroup");
    if (itemSpawnPointsGroup->Attribute("name") != std::string("Item Layer")) {
        for (auto* objGroup = itemSpawnPointsGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Item Layer")) {
                itemSpawnPointsGroup = objGroup;
                break;
            }
        }
    }
    if (itemSpawnPointsGroup->Attribute("name") == std::string("Item Layer")) {
        for (auto* obj = itemSpawnPointsGroup->FirstChildElement("object");
        obj != nullptr;
        obj = obj->NextSiblingElement("object")) {
            Vector2D pos;
            pos.x = obj->FloatAttribute("x");
            pos.y = obj->FloatAttribute("y");
            itemSpawnPoints.push_back(pos);
        }
    }
}

void Map::draw(const Camera &cam) {
    SDL_FRect src{}, dest{};

    dest.w = dest.h = 32;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int type = tileData[row][col];

            float worldX = static_cast<float>(col) * dest.w;
            float worldY = static_cast<float>(row) * dest.h;

            // Move the tiles or map relative to the camera
            // Convert from world space to screen space
            dest.x = std::round(worldX - cam.view.x);
            dest.y = std::round(worldY - cam.view.y);

            switch (type) {
                case 1:
                    // Dirt
                    src.x = 0;
                    src.y = 0;
                    src.w = 32;
                    src.h = 32;
                    break;
                case 2:
                    // Grass
                    src.x = 32;
                    src.y = 0;
                    src.w = 32;
                    src.h = 32;
                    break;
                case 4:
                    // Water
                    src.x = 32;
                    src.y = 32;
                    src.w = 32;
                    src.h = 32;
                    break;
                default:
                    break;
            }
            TextureManager::draw(tileset, src, dest);
        }
    }
}
