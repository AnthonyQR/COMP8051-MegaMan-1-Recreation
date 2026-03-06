//
// Created by antho on 2026-01-14.
//

#include "GameObject.h"

#include <iostream>
#include <ostream>

#include "TextureManager.h"

GameObject::GameObject(const char *path, float x, float y) :
xPos(x), yPos(y), texture(TextureManager::load(path))
{}

GameObject::~GameObject() {
    if (texture) SDL_DestroyTexture(texture);
}

void GameObject::update(float deltaTime) {
    // Move GameObject depending on speed & deltaTime
    float speed = 60;
    xPos += speed * deltaTime;
    yPos += speed * deltaTime;

    // Where do we want to crop the texture from?
    srcRect.x = srcRect.y = 0;

    // How much to crop?
    srcRect.w = srcRect.h = 32;

    // Where to draw it?
    dstRect.x = static_cast<float>(xPos);
    dstRect.y = static_cast<float>(yPos);

    // What sizes to render?
    dstRect.h = srcRect.h;
    dstRect.w = srcRect.w;
}

void GameObject::draw() {
    TextureManager::draw(texture, srcRect, dstRect);
}
