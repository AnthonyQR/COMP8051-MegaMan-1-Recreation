//
// Created by antho on 2026-04-07.
//

#ifndef TUTORIAL1_UPDATESCENESTATESYSTEM_H
#define TUTORIAL1_UPDATESCENESTATESYSTEM_H
#include <memory>

#include "Component.h"
#include "Entity.h"

class UpdateSceneStateSystem {
public:
    void update(const std::unique_ptr<Entity>& entity, SceneState& currentState);
};

#endif //TUTORIAL1_UPDATESCENESTATESYSTEM_H