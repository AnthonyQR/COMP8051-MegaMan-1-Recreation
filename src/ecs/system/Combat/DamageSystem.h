//
// Created by antho on 2026-03-23.
//

#ifndef TUTORIAL1_DAMAGESYSTEM_H
#define TUTORIAL1_DAMAGESYSTEM_H
#include <memory>
#include <vector>

#include "Entity.h"

class World;

class DamageSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, World& world);
};

#endif //TUTORIAL1_DAMAGESYSTEM_H