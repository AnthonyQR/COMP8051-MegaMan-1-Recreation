//
// Created by antho on 2026-03-23.
//

#ifndef TUTORIAL1_DAMAGESYSTEM_H
#define TUTORIAL1_DAMAGESYSTEM_H
#include <memory>
#include <vector>

#include "Entity.h"

class DamageSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities);
};

#endif //TUTORIAL1_DAMAGESYSTEM_H