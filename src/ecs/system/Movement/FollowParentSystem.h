//
// Created by antho on 2026-03-17.
//

#ifndef TUTORIAL1_FOLLOWENTITYSYSTEM_H
#define TUTORIAL1_FOLLOWENTITYSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class FollowParentSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            if (e->hasComponent<FollowParent>() &&
                e->hasComponent<Parent>() &&
                e->hasComponent<Transform>()) {
                auto& parent = e->getComponent<Parent>().parent;
                auto& followParentStats = e->getComponent<FollowParent>();

                auto& parentTransform = parent->getComponent<Transform>();
                float xOffset = followParentStats.xOffset;
                float yOffset = followParentStats.yOffset;

                auto& transform = e->getComponent<Transform>();
                transform.position.x = parentTransform.position.x + xOffset;
                transform.position.y = parentTransform.position.y + yOffset;
            }
        }
    }
};

#endif //TUTORIAL1_FOLLOWENTITYSYSTEM_H