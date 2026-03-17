//
// Created by antho on 2026-02-18.
//

#ifndef TUTORIAL1_ANIMATIONSYSTEM_H
#define TUTORIAL1_ANIMATIONSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

#include "./AnimationSystems/PlayerAnimationSystem.h"

// State System: Deciding which clip to use
// Check if the animation has been switched
// Playback System: Advances the animation

class AnimationSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& e : entities) {
            if (e->hasComponent<Animation>() && e->hasComponent<Velocity>()) {
                std::string newClip;
                auto& anim = e->getComponent<Animation>();
                auto& velocity = e->getComponent<Velocity>();

                if (e->hasComponent<PlayerTag>()) {
                    newClip = PlayerAnimationSystem::getAnimationClip(e);
                }
                else {
                    return;
                }
                /*
                auto& anim = e->getComponent<Animation>();
                auto& velocity = e->getComponent<Velocity>();

                // State System

                if (velocity.direction.x > 0.0f) {
                    newClip = "walk_right";
                }
                else if (velocity.direction.x < 0.0f) {
                    newClip = "walk_left";
                }
                else if (velocity.direction.y > 0.0f) {
                    newClip = "walk_down";
                }
                else if (velocity.direction.y < 0.0f) {
                    newClip = "walk_up";
                }
                else {
                    if (anim.currentClip == "idle_left" ||
                        anim.currentClip == "idle_right" ||
                        anim.currentClip == "idle_down" ||
                        anim.currentClip == "idle_up") {
                        newClip = anim.currentClip;
                    }
                    else if (anim.currentClip == "walk_right") {
                        newClip = "idle_right";
                    }
                    else if (anim.currentClip == "walk_left") {
                        newClip = "idle_left";
                    }
                    else if (anim.currentClip == "walk_down") {
                        newClip = "idle_down";
                    }
                    else{
                        newClip = "idle_up";
                    }

                }
                */

                // Check if the animation has switched
                // If the chosen clip is different from the current one, switch to new clip, reset time & frame index
                // Also check if the new clip exists as an animation & skip if it doesn't exist
                if (newClip != anim.currentClip && anim.clips.contains(newClip)) {
                    anim.currentClip = newClip; // Switch to new clip
                    anim.time = 0.0f; // Reset time to 0d
                    anim.currentFrame = 0; // Reset frame index to 0d
                }

                // Playback System: Advance the animation
                float animFrameSpeed = anim.speed; // How long each animation frame should last
                auto clip = anim.clips[anim.currentClip]; // Retrieve the frame data from the current clip
                // Advance time
                anim.time += dt; // Every game loop frame, add the accumulated time

                // Once enough time has passed
                if (anim.time >= animFrameSpeed) {
                    anim.time -= animFrameSpeed; // Subtract animFrameSpeed (ex. 0.1f) so the extra time isn't lost

                    std::size_t totalAnimationFrames = clip.frameIndices.size();
                    // Advance to the next animation frame (currentAnimationFrame + 1)
                    // Wrap around % so it loops when reaching the end of the clip
                    anim.currentFrame = (anim.currentFrame + 1) % totalAnimationFrames;
                }


            }
        }
    }
};

#endif //TUTORIAL1_ANIMATIONSYSTEM_H