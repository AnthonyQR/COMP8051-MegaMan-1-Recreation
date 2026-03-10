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

    if (!collidersObjectGroup) return;

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
            c.rect.x = obj->FloatAttribute("x") * 3;
            c.rect.y = obj->FloatAttribute("y") * 3;
            c.rect.w = obj->FloatAttribute("width") * 3;
            c.rect.h = obj->FloatAttribute("height") * 3;
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

            /*
            switch (type) {
                case 1:
                    src.x = 0;
                    src.y = 0;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 2:
                    src.x = 16;
                    src.y = 0;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 3:
                    src.x = 32;
                    src.y = 0;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 4:
                    src.x = 48;
                    src.y = 0;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 5:
                    src.x = 64;
                    src.y = 0;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 6:
                    src.x = 80;
                    src.y = 0;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 7:
                    src.x = 0;
                    src.y = 16;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 8:
                    src.x = 16;
                    src.y = 16;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 9:
                    src.x = 32;
                    src.y = 16;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 10:
                    src.x = 48;
                    src.y = 16;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 11:
                    src.x = 64;
                    src.y = 16;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 12:
                    src.x = 80;
                    src.y = 16;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 13:
                    src.x = 0;
                    src.y = 32;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 14:
                    src.x = 16;
                    src.y = 32;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 15:
                    src.x = 32;
                    src.y = 32;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 16:
                    src.x = 48;
                    src.y = 32;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 17:
                    src.x = 64;
                    src.y = 32;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 18:
                    src.x = 80;
                    src.y = 32;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 19:
                    src.x = 0;
                    src.y = 48;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 20:
                    src.x = 16;
                    src.y = 48;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 21:
                    src.x = 32;
                    src.y = 48;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 22:
                    src.x = 48;
                    src.y = 48;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 23:
                    src.x = 64;
                    src.y = 48;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 24:
                    src.x = 80;
                    src.y = 48;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 25:
                    src.x = 0;
                    src.y = 64;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 26:
                    src.x = 16;
                    src.y = 64;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 27:
                    src.x = 32;
                    src.y = 64;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 28:
                    src.x = 48;
                    src.y = 64;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 29:
                    src.x = 64;
                    src.y = 64;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 30:
                    src.x = 80;
                    src.y = 64;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 31:
                    src.x = 0;
                    src.y = 80;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 32:
                    src.x = 16;
                    src.y = 80;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 33:
                    src.x = 32;
                    src.y = 80;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 34:
                    src.x = 48;
                    src.y = 80;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 35:
                    src.x = 64;
                    src.y = 80;
                    src.w = 16;
                    src.h = 16;
                    break;
                case 36:
                    src.x = 80;
                    src.y = 80;
                    src.w = 16;
                    src.h = 16;
                    break;
                default:
                    break;
            }
        */

            TextureManager::draw(tileset, src, dest);
        }
    }
}
