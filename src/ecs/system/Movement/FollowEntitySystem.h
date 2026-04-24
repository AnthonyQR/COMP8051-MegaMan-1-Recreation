//
// Created by antho on 2026-03-17.
//

#ifndef TUTORIAL1_FOLLOWENTITYSYSTEM_H
#define TUTORIAL1_FOLLOWENTITYSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class FollowEntitySystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            if (e->hasComponent<FollowEntity>() && e->hasComponent<Transform>()) {
                auto& followEntityStats = e->getComponent<FollowEntity>();
                auto& followedEntity = followEntityStats.followedEntity;

                auto& followedEntityTransform = followedEntity->getComponent<Transform>();
                float xOffset = followEntityStats.xOffset;
                float yOffset = followEntityStats.yOffset;
                auto& transform = e->getComponent<Transform>();

                transform.position.x = followedEntityTransform.position.x + xOffset;
                transform.position.y = followedEntityTransform.position.y + yOffset;
            }
        }
    }
};

#endif //TUTORIAL1_FOLLOWENTITYSYSTEM_H