//
// Created by antho on 2026-05-02.
//

#ifndef MEGAMAN_SCREENFREEZETIMERSYSTEM_H
#define MEGAMAN_SCREENFREEZETIMERSYSTEM_H

#include <memory>
#include <vector>

#include "Entity.h"

class ScreenFreezeTimerSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt);
};

#endif //MEGAMAN_SCREENFREEZETIMERSYSTEM_H