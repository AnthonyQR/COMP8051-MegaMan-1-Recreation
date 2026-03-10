//
// Created by antho on 2026-01-21.
//

#ifndef TUTORIAL1_MOVEMENTSYSTEM_H
#define TUTORIAL1_MOVEMENTSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class MovementSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>()) {
                auto& t = entity->getComponent<Transform>();
                auto& v = entity->getComponent<Velocity>();

                // Track previous frame's position
                t.oldPosition = t.position;

                Vector2D directionVector = v.direction;

                // Normalizing
                // directionVector.normalize();

                // Vector 2D needs an operator function to multiply a float to itself
                Vector2D velocityVector = directionVector * v.speed;

                t.position += (velocityVector * dt);
            }
        }
    }
};

#endif //TUTORIAL1_MOVEMENTSYSTEM_H