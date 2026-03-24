//
// Created by antho on 2026-03-23.
//

#ifndef TUTORIAL1_AUTOFIRINGSYSTEM_H
#define TUTORIAL1_AUTOFIRINGSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class AutoFiringSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float dt) {
        for (auto& entity: entities) {
            if (entity->hasComponent<IsFiring>() && entity->hasComponent<AutoFiring>()) {
                auto& isFiring = entity->getComponent<IsFiring>().firing;
                auto& autoFiring = entity->getComponent<AutoFiring>();
            }
        }
    }
};


#endif //TUTORIAL1_AUTOFIRINGSYSTEM_H