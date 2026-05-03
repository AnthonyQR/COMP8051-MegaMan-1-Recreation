//
// Created by antho on 2026-05-02.
//

#include "HealOvertimeSystem.h"

#include "Component.h"

void HealOvertimeSystem::update(const std::vector<std::unique_ptr<Entity> > &entities, World &world, float dt) {
    for (auto& entity : entities) {
        if (entity->hasComponent<HealOvertime>()) {
            auto& healOvertime = entity->getComponent<HealOvertime>();
            auto& healedEntity = healOvertime.healTarget;

            if (healedEntity == nullptr) {
                entity->destroy();
                continue;
            }
            if (!healedEntity->isActive()) {
                entity->destroy();
                continue;
            }
            if (!healedEntity->hasComponent<Health>()) {
                entity->destroy();
                continue;
            }

            auto& health = healedEntity->getComponent<Health>();
            if (health.currentHealth >= health.maxHealth) {
                entity->destroy();
                continue;
            }

            healOvertime.timer -= dt;
            if (healOvertime.timer <= 0) {
                healOvertime.timer = healOvertime.healDelay;
                health.currentHealth++;
                healOvertime.healAmount--;

                if (healOvertime.healAmount <= 0) {
                    entity->destroy();
                }
                if (healedEntity->hasComponent<OnHealCallback>()) {
                    healedEntity->getComponent<OnHealCallback>().callback(healedEntity);
                }
            }
        }
    }
}
