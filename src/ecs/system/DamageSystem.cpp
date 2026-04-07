//
// Created by antho on 2026-03-23.
//

#include "DamageSystem.h"

#include "Component.h"
#include "Game.h"
#include "ERS/OnDestroyEvent.h"

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

            if (damage.damageDealerEntity->hasComponent<PlayerTag>() && damage.damageDealerEntity->hasComponent<ProjectileTag>()) {
                OnDestroyEvent::onDestroy(damage.damageDealerEntity, world);
                damage.damageDealerEntity->destroy();
            }

            if (damage.damagedEntity->hasComponent<Invulnerability>() && !damage.invulIgnore) {
                auto& invulnerability = damage.damagedEntity->getComponent<Invulnerability>();
                if (invulnerability.isInvulnerable) {
                    if (!damage.damagedEntity->hasComponent<PlayerTag>()) {
                        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("dink"));
                    }
                    entity -> destroy();
                    continue;
                }
            }

            auto& health = damage.damagedEntity->getComponent<Health>();
            health.currentHealth -= damage.damage;

            if (health.currentHealth < 0) {
                health.currentHealth = 0;
            }

            if (health.currentHealth <= 0) {
                world.getEventManager().emit(DestroyedEvent(damage.damagedEntity));
                if (damage.damagedEntity->hasComponent<OnDeathCallback>()) {
                    damage.damagedEntity->getComponent<OnDeathCallback>().callback(damage.damagedEntity);
                }

                if (damage.damagedEntity->hasComponent<Children>()) {
                    for (auto& child : damage.damagedEntity->getComponent<Children>().children) {
                        child->destroy();
                    }
                }
                auto& damagedEntity = damage.damagedEntity;
                damagedEntity->destroy();
            }
            else {
                if (damage.damagedEntity->hasComponent<OnHitCallback>()) {
                    damage.damagedEntity->getComponent<OnHitCallback>().callback(damage.damagedEntity, damage.damageDealerEntity);
                }
            }
            entity -> destroy();
        }
    }
}
