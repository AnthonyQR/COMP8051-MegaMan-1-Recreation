//
// Created by antho on 2026-03-24.
//

#ifndef TUTORIAL1_SCENETRANSITIONDELAYSYSTEM_H
#define TUTORIAL1_SCENETRANSITIONDELAYSYSTEM_H
#include <memory>
#include <vector>

#include "Entity.h"


class SceneTransitionDelaySystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt);
};

#endif //TUTORIAL1_SCENETRANSITIONDELAYSYSTEM_H