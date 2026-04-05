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

            if (damage.damagedEntity == nullptr) {
                entity->destroy();
                continue;
            }

            if (!damage.damagedEntity->hasComponent<Health>()) {
                entity->destroy();
                continue;
            }

            auto& health = damage.damagedEntity->getComponent<Health>();
            health.currentHealth -= damage.damage;

            if (health.currentHealth < 0) {
                health.currentHealth = 0;
            }

            if (damage.damagedEntity->hasComponent<PlayerTag>()) {
                Game::gameState.playerHealth = health.currentHealth;
                std::cout << "Health: " << health.currentHealth << std::endl;
                world.getAudioEventQueue().push(std::make_unique<AudioEvent>("megamanDamage"));

                auto& ladderClimbing = damage.damagedEntity->getComponent<LadderClimbing>();
                ladderClimbing.isClimbing = false;
                auto& gravity = damage.damagedEntity->getComponent<Gravity>();
                gravity.gravityEnabled = true;

                // Find the health bar & update it
                for (auto& e : entities) {
                    if (e->hasComponent<HealthBarUpdate>()) {
                        e->getComponent<HealthBarUpdate>().callback(e.get());
                        break;
                    };
                }
            }


            if (health.currentHealth <= 0) {
                world.getEventManager().emit(DestroyedEvent(damage.damagedEntity));
                if (damage.damagedEntity->hasComponent<OnDeathCallback>()) {
                    damage.damagedEntity->getComponent<OnDeathCallback>().callback(damage.damagedEntity);
                }

                if (damage.damagedEntity->hasComponent<PlayerTag>()) {
                    Game::gameState.lives--;
                    std::cout << "Lives: " << Game::gameState.lives << std::endl;
                    auto& transition (world.createEntity());
                    transition.addComponent<SceneTransitionDelay>(2.25f, "cutman");
                    world.getAudioEventQueue().push(std::make_unique<AudioEvent>("megamanDefeat"));
                    Game::checkSceneState();
                }
                if (damage.damagedEntity->hasComponent<Children>()) {
                    for (auto& child : damage.damagedEntity->getComponent<Children>().children) {
                        child->destroy();
                    }
                }
                auto& damagedEntity = damage.damagedEntity;
                damagedEntity->destroy();
            }
            entity -> destroy();
        }
    }
}
