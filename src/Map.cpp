//
// Created by antho on 2026-01-14.
//

#include "Map.h"

#include <cmath>
#include <iostream>

#include "TextureManager.h"
#include <sstream>
#include <tinyxml2.h>

#include "AspectRatioUtil.h"

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
    auto* collidersObjectGroup = layer->NextSiblingElement("objectgroup"); // Set to the first object group

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
            c.rect.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            c.rect.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            c.rect.w = obj->FloatAttribute("width") * AspectRatioUtil::horizontalAspectMult();
            c.rect.h = obj->FloatAttribute("height") * AspectRatioUtil::verticalAspectMult();
            colliders.push_back(c);
            }
    }

    // Parse Ladders
    auto* laddersObjectGroup = layer->NextSiblingElement("objectgroup");
    if (laddersObjectGroup->Attribute("name") != std::string("Ladder Layer")) {
        for (auto* objGroup = laddersObjectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Ladder Layer")) {
                laddersObjectGroup = objGroup;
                break;
            }
        }
    }
    if (laddersObjectGroup->Attribute("name") == std::string("Ladder Layer")) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = laddersObjectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment

            Collider c;
            c.rect.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            c.rect.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            c.rect.w = obj->FloatAttribute("width") * AspectRatioUtil::horizontalAspectMult();
            c.rect.h = obj->FloatAttribute("height") * AspectRatioUtil::verticalAspectMult();
            ladders.push_back(c);
            }
    }

    // Parse beak enemy spawn points
    auto* beakEnemyLeftObjectGroup = layer->NextSiblingElement("objectgroup");
    if (beakEnemyLeftObjectGroup->Attribute("name") != std::string("Beak Left Spawn Layer")) {
        for (auto* objGroup = beakEnemyLeftObjectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Beak Left Spawn Layer")) {
                beakEnemyLeftObjectGroup = objGroup;
                break;
            }
        }
    }
    if (beakEnemyLeftObjectGroup->Attribute("name") == std::string("Beak Left Spawn Layer")) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = beakEnemyLeftObjectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment

            Vector2D pos;
            pos.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            pos.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            beakEnemyLeftSpawnPoints.push_back(pos);
            }
    }
    auto* beakEnemyRightObjectGroup = layer->NextSiblingElement("objectgroup");
    if (beakEnemyRightObjectGroup->Attribute("name") != std::string("Beak Right Spawn Layer")) {
        for (auto* objGroup = beakEnemyRightObjectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Beak Right Spawn Layer")) {
                beakEnemyRightObjectGroup = objGroup;
                break;
            }
        }
    }
    if (beakEnemyRightObjectGroup->Attribute("name") == std::string("Beak Right Spawn Layer")) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = beakEnemyRightObjectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment

            Vector2D pos;
            pos.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            pos.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            beakEnemyRightSpawnPoints.push_back(pos);
            }
    }

    auto* beakEnemyRightEarlyObjectGroup = layer->NextSiblingElement("objectgroup");
    if (beakEnemyRightEarlyObjectGroup->Attribute("name") != std::string("Beak Right Early Spawn Layer")) {
        for (auto* objGroup = beakEnemyRightEarlyObjectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Beak Right Early Spawn Layer")) {
                beakEnemyRightEarlyObjectGroup = objGroup;
                break;
            }
        }
    }
    if (beakEnemyRightEarlyObjectGroup->Attribute("name") == std::string("Beak Right Early Spawn Layer")) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = beakEnemyRightEarlyObjectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment

            Vector2D pos;
            pos.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            pos.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            beakEnemyRightEarlySpawnPoints.push_back(pos);
            }
    }

    auto* bladerEnemyObjectGroup = layer->NextSiblingElement("objectgroup");
    if (bladerEnemyObjectGroup->Attribute("name") != std::string("Blader Enemy Spawn Layer")) {
        for (auto* objGroup = bladerEnemyObjectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Blader Enemy Spawn Layer")) {
                bladerEnemyObjectGroup = objGroup;
                break;
            }
            }
    }
    if (bladerEnemyObjectGroup->Attribute("name") == std::string("Blader Enemy Spawn Layer")) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = bladerEnemyObjectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment

            Vector2D pos;
            pos.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            pos.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            bladerEnemySpawnPoints.push_back(pos);
            }
    }

    // Parse camera bounds
    auto* cameraBoundsObjectGroup = layer->NextSiblingElement("objectgroup");
    if (cameraBoundsObjectGroup->Attribute("name") != std::string("Camera Bounds Layer")) {
        for (auto* objGroup = cameraBoundsObjectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Camera Bounds Layer")) {
                cameraBoundsObjectGroup = objGroup;
                break;
            }
        }
    }
    if (cameraBoundsObjectGroup->Attribute("name") == std::string("Camera Bounds Layer")) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = cameraBoundsObjectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment

            Collider c;
            c.rect.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            c.rect.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            c.rect.w = obj->FloatAttribute("width") * AspectRatioUtil::horizontalAspectMult();
            c.rect.h = obj->FloatAttribute("height") * AspectRatioUtil::verticalAspectMult();
            cameraBounds.push_back(c);
            }
    }

    // Parse Death Colliders
    auto* deathCollidersObjectGroup = layer->NextSiblingElement("objectgroup");
    if (deathCollidersObjectGroup->Attribute("name") != std::string("Death Collision Layer")) {
        for (auto* objGroup = deathCollidersObjectGroup->NextSiblingElement("objectgroup");
            objGroup != nullptr;
            objGroup = objGroup->NextSiblingElement("objectgroup")) {
            if (objGroup->Attribute("name") == std::string("Death Collision Layer")) {
                deathCollidersObjectGroup = objGroup;
                break;
            }
        }
    }
    if (deathCollidersObjectGroup->Attribute("name") == std::string("Death Collision Layer")) {
        // Create a for loop with initialization, condition and an increment
        for (auto* obj = deathCollidersObjectGroup->FirstChildElement("object"); // Initialization
            obj != nullptr; // Condition
            obj = obj->NextSiblingElement("object")) { // Increment

            Collider c;
            c.rect.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            c.rect.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            c.rect.w = obj->FloatAttribute("width") * AspectRatioUtil::horizontalAspectMult();
            c.rect.h = obj->FloatAttribute("height") * AspectRatioUtil::verticalAspectMult();
            deathColliders.push_back(c);
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
            pos.x = obj->FloatAttribute("x") * AspectRatioUtil::horizontalAspectMult();
            pos.y = obj->FloatAttribute("y") * AspectRatioUtil::verticalAspectMult();
            itemSpawnPoints.push_back(pos);
        }
    }
}

void Map::draw(const Camera &cam) {
    SDL_FRect src{}, dest{};

    dest.w = 16 * AspectRatioUtil::horizontalAspectMult();
    dest.h = 16 * AspectRatioUtil::verticalAspectMult();

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
