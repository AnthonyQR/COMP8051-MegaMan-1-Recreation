//
// Created by antho on 2026-03-29.
//

#ifndef TUTORIAL1_UIRENDERSYSTEM_H
#define TUTORIAL1_UIRENDERSYSTEM_H
#include <algorithm>
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "RenderUtils.h"
#include "TextureManager.h"

class UIRenderSystem {
public:
    void render(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Sprite>()) {
                auto& transform = entity->getComponent<Transform>();
                auto& sprite = entity->getComponent<Sprite>();

                if (sprite.renderLayer != RenderLayer::UI) continue;

                // No converting from world space to screen space
                sprite.dst.x = transform.position.x;
                sprite.dst.y = transform.position.y;

                if (sprite.isVisible) {
                    SDL_FRect scaledDest = RenderUtils::getScaledDest(sprite.dst, transform.scale);
                    TextureManager::draw(sprite.texture, sprite.src, scaledDest);
                }
            }
        }
    }
};

#endif //TUTORIAL1_UIRENDERSYSTEM_H