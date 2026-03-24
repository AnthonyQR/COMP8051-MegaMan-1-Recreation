//
// Created by antho on 2026-03-23.
//

#include "DamageSystem.h"

#include "Component.h"
#include "Game.h"

void DamageSystem::update(const std::vector<std::unique_ptr<Entity>>& entities) {
    for (auto& entity: entities) {
        if (entity->hasComponent<Damage>()) {
            auto& damage = entity->getComponent<Damage>();

            if (damage.damagedEntity != nullptr && damage.damagedEntity->hasComponent<Health>()) {
                auto& health = damage.damagedEntity->getComponent<Health>();
                health.currentHealth -= damage.damage;

                if (damage.damagedEntity->hasComponent<PlayerTag>()) {
                    Game::gameState.playerHealth = health.currentHealth;
                    std::cout << health.currentHealth << std::endl;
                }

                if (health.currentHealth <= 0) {
                    damage.damagedEntity->destroy();
                    if (damage.damagedEntity->hasComponent<PlayerTag>()) {
                        Game::onSceneChangeRequest("gameover");
                    }
                }
            }
            entity -> destroy();
        }
    }
}
