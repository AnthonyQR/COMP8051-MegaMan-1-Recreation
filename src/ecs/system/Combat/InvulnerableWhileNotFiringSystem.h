//
// Created by antho on 2026-03-24.
//

#ifndef TUTORIAL1_INVULNERABLEWHILEFIRINGSYSTEM_H
#define TUTORIAL1_INVULNERABLEWHILEFIRINGSYSTEM_H
#include <iostream>
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class InvulnerableWhileNotFiringSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<IsFiring>() &&
                entity->hasComponent<Invulnerability>() &&
                entity->hasComponent<InvulnerableWhileNotFiring>())
            {
                auto& isFiring = entity->getComponent<IsFiring>();
                auto& invulnerability = entity->getComponent<Invulnerability>();

                if (isFiring.startFiring || isFiring.firing || isFiring.endFiring) {
                    invulnerability.isInvulnerable = false;
                }
                else {
                    invulnerability.isInvulnerable = true;
                }
            }
        }
    }
};
#endif //TUTORIAL1_INVULNERABLEWHILEFIRINGSYSTEM_H