//
// Created by antho on 2026-02-25.
//

#ifndef TUTORIAL1_DESTRUCTIONSYSTEM_H
#define TUTORIAL1_DESTRUCTIONSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class World;

class DestructionSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, World& world);
};

#endif //TUTORIAL1_DESTRUCTIONSYSTEM_H