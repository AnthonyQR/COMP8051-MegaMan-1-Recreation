//
// Created by antho on 2026-02-04.
//

#ifndef TUTORIAL1_COLLISIONSYSTEN_H
#define TUTORIAL1_COLLISIONSYSTEN_H
#include <algorithm>
#include <memory>
#include <set>
#include <vector>
#include "Entity.h"

using CollisionKey = std::pair<Entity*, Entity*>;

// Forward declaring
class World;

class CollisionSystem {
public:
    void update(World& world);
    void cleanupDestroyedCollisions(const Entity* entity);
    std::set<CollisionKey> collisions;
private:
    std::vector<Entity*> queryCollidables(const std::vector<std::unique_ptr<Entity>>& entities);
    CollisionKey makeKey(Entity* entityA, Entity* entityB) {
        // Automatically orders our pair so smaller number (memory address) is ordered first
        return std::minmax(entityA, entityB);
    };
};

#endif //TUTORIAL1_COLLISIONSYSTEN_H