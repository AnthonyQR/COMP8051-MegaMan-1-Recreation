//
// Created by antho on 2026-04-28.
//

#ifndef MEGAMAN_TRACKPLAYER_H
#define MEGAMAN_TRACKPLAYER_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class TrackPlayerSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities) {
        Entity* player = nullptr;
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerTag>() &&
                entity->hasComponent<MainPlayer>()) {
                player = entity.get();
                }
        }
        if (player == nullptr) return;
        auto& playerTransform = player->getComponent<Transform>();

        for (auto& entity : entities) {
            if (entity->hasComponent<TrackPlayer>() &&
                entity->hasComponent<Transform>()) {
                auto& trackPlayer = entity->getComponent<TrackPlayer>();
                auto& transform = entity->getComponent<Transform>();

                if (playerTransform.position.x > transform.position.x) {
                    trackPlayer.isPlayerToTheRight = true;
                }
                else {
                    trackPlayer.isPlayerToTheRight = false;
                }

                if (playerTransform.position.y > transform.position.y) {
                    trackPlayer.isPlayerBelow = true;
                }
                else {
                    trackPlayer.isPlayerBelow = false;
                }
            }
        }
    }
};

#endif //MEGAMAN_TRACKPLAYER_H