//
// Created by antho on 2026-02-18.
//

#ifndef TUTORIAL1_ASSETMANAGER_H
#define TUTORIAL1_ASSETMANAGER_H
#include <unordered_map>

#include "Component.h"

class AssetManager {
    static std::unordered_map<std::string, Animation> animations;
    static Animation loadAnimationFromXML(const char* path);

public:
    static void loadAnimation(const std::string& clipName, const char* path);
    static const Animation& getAnimation(const std::string& clipName);
};

#endif //TUTORIAL1_ASSETMANAGER_H