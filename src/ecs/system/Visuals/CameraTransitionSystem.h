//
// Created by antho on 2026-05-03.
//

#ifndef MEGAMAN_CAMERATRANSITIONSYSTEM_H
#define MEGAMAN_CAMERATRANSITIONSYSTEM_H
#include <memory>
#include <vector>

#include "Entity.h"

class CameraTransitionSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt);
};

#endif //MEGAMAN_CAMERATRANSITIONSYSTEM_H