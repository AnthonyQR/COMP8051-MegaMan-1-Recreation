//
// Created by antho on 2026-02-18.
//

#ifndef TUTORIAL1_ANIMATIONSYSTEM_H
#define TUTORIAL1_ANIMATIONSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

#include "PlayerAnimationSystem.h"
#include "BeakAnimationSystem.h"

// State System: Deciding which clip to use
// Check if the animation has been switched
// Playback System: Advances the animation

class AnimationSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& e : entities) {
            if (e->hasComponent<Animation>()) {
                NextAnimationClip newClip;
                auto& anim = e->getComponent<Animation>();

                if (e->hasComponent<PlayerTag>()) {
                    newClip = PlayerAnimationSystem::getAnimationClip(e);
                }
                else if (e->hasComponent<BeakEnemyTag>()) {
                    newClip = BeakAnimationSystem::getAnimationClip(e);
                }
                else {
                    continue;
                }

                // Check if the animation has switched
                // If the chosen clip is different from the current one, switch to new clip, reset time & frame index
                // Also check if the new clip exists as an animation & skip if it doesn't exist
                if (newClip.name != anim.currentClip && anim.clips.contains(newClip.name)) {
                    anim.currentClip = newClip.name; // Switch to new clip
                    anim.time = 0.0f; // Reset time to 0d
                    anim.currentFrame = 0; // Reset frame index to 0d
                }

                // Playback System: Advance the animation
                float animFrameSpeed = newClip.animationSpeed; // How long each animation frame should last
                auto clip = anim.clips[anim.currentClip]; // Retrieve the frame data from the current clip

                // Don't advance animation if frame speed is at 0 or below
                if (animFrameSpeed <= 0) continue;

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