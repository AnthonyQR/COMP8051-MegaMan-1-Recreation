//
// Created by antho on 2026-02-04.
//

#ifndef TUTORIAL1_COLLISIONSYSTEN_H
#define TUTORIAL1_COLLISIONSYSTEN_H
#include <memory>
#include <vector>

#include "Entity.h"

// Forward declaring
class World;

class CollisionSystem {
public:
    void update(World& world);
private:
    std::vector<Entity*> queryCollidables(const std::vector<std::unique_ptr<Entity>>& entities);
};

#endif //TUTORIAL1_COLLISIONSYSTEN_H