//
// Created by antho on 2026-03-23.
//

#include "DamageSystem.h"

#include "Component.h"
#include "Game.h"

void DamageSystem::update(const std::vector<std::unique_ptr<Entity>>& entities, World& world) {
    for (auto& entity: entities) {
        if (entity->hasComponent<Damage>()) {
            auto& damage = entity->getComponent<Damage>();

            if (damage.damagedEntity != nullptr && damage.damagedEntity->hasComponent<Health>()) {
                // Check for invulnerability
                if (damage.damagedEntity->hasComponent<Invulnerability>()) {
                    auto& invulnerability = damage.damagedEntity->getComponent<Invulnerability>();
                    if (invulnerability.isInvulnerable) {
                        entity -> destroy();
                        continue;
                    }
                }

                auto& health = damage.damagedEntity->getComponent<Health>();
                health.currentHealth -= damage.damage;

                if (damage.damagedEntity->hasComponent<PlayerTag>()) {
                    Game::gameState.playerHealth = health.currentHealth;
                    std::cout << "Health: " << health.currentHealth << std::endl;
                }

                if (health.currentHealth <= 0) {
                    damage.damagedEntity->destroy();
                    if (damage.damagedEntity->hasComponent<PlayerTag>()) {
                        Game::gameState.lives--;
                        std::cout << "Lives: " << Game::gameState.lives << std::endl;
                        auto& transition (world.createEntity());
                        transition.addComponent<SceneTransitionDelay>(2.25f, "cutman");
                    }
                }
            }
            entity -> destroy();
        }
    }
}
