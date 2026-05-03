//
// Created by antho on 2026-05-02.
//

#ifndef MEGAMAN_HEALOVERTIMESYSTEM_H
#define MEGAMAN_HEALOVERTIMESYSTEM_H
#include <memory>
#include <vector>

#include "Entity.h"

class World;

class HealOvertimeSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, World& world, float dt);
};

#endif //MEGAMAN_HEALOVERTIMESYSTEM_H