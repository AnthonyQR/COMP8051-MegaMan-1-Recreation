//
// Created by antho on 2026-03-31.
//

#ifndef TUTORIAL1_BLADERATTACKSYSTEM_H
#define TUTORIAL1_BLADERATTACKSYSTEM_H
#include <cmath>
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class BladerAttackSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<BladerEnemyTag>()) {
                auto& bladerAttack = entity->getComponent<BladerAttack>();
                auto& velocity = entity->getComponent<Velocity>();
                auto& transform = entity->getComponent<Transform>();

                if (!bladerAttack.isAttacking) continue;

                bladerAttack.timer -= dt;
                velocity.xSpeed += bladerAttack.xAcceleration * dt;
                velocity.ySpeed += bladerAttack.yAcceleration * dt;

                if (bladerAttack.timer <= 0) {
                    if (bladerAttack.firstPhase) {
                        bladerAttack.firstPhase = false;
                        bladerAttack.secondPhase = true;

                        bladerAttack.timer = bladerAttack.totalAttackDuration - bladerAttack.firstAttackDuration;

                        bladerAttack.yAcceleration = 0;
                        velocity.ySpeed = (bladerAttack.initialYPosition - transform.position.y) / bladerAttack.timer;

                        bladerAttack.xAcceleration = 0;
                        velocity.xSpeed = (bladerAttack.secondPhaseXDistance / bladerAttack.timer) * velocity.direction.x;
                    }
                    else if (bladerAttack.secondPhase) {
                        bladerAttack.secondPhase = false;
                        bladerAttack.isAttacking = false;

                        velocity.xSpeed = 0;
                        velocity.ySpeed = 0;

                        entity->getComponent<MoveTowardsPlayer>().isMovingTowards = true;
                    }
                }
            }
        }
    }
};

#endif //TUTORIAL1_BLADERATTACKSYSTEM_H