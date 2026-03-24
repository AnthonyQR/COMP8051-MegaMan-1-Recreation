//
// Created by antho on 2026-01-21.
//

#ifndef TUTORIAL1_COMPONENT_H
#define TUTORIAL1_COMPONENT_H
#include <functional>
#include <SDL3/SDL_render.h>

#include "Vector2D.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "AnimationClip.h"
#include "Entity.h"

struct Transform {
    Vector2D position{};
    float rotation{};
    float scale{};
    Vector2D oldPosition{};
};

struct Velocity {
    Vector2D direction{};
    float xSpeed{};
    float ySpeed{};
};

struct Gravity {
    float gravitySpeed{};
    float fallingCap{};
    bool gravityEnabled{};
};

struct IsGrounded {
    bool grounded{};
};

struct IsFacingRight {
    bool facingRight = true;
};

struct Jump {
    float jumpSpeed{};
    float fastFallSpeed{};
    bool hasJumped = false;
    bool fastFalling = false;
};

struct Sprite {
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
};

struct Collider {
    std::string tag;
    float xOffset{};
    float yOffset{};
    SDL_FRect rect{};
};

struct LadderClimbing {
    float climbSpeed{};
    bool canClimb = false;
    bool isClimbing = false;
    Entity* ladderEntity = nullptr;
};

struct Animation {
    std::unordered_map<std::string, AnimationClip> clips{};
    std::string currentClip{};
    float time{}; // Time is accumulated for the current frame
    int currentFrame{}; // Index of the current frame in the clip
    float speed = 0.12f; // Time per frame
};

struct Camera {
    SDL_FRect view;
    float worldWidth;
    float worldHeight;
};

struct TimedSpawner {
    float spawnInterval{};
    std::function<void()> spawnCallback{};
    float timer{};
};

struct SpawnOnVisible {

};

// Our game state, might have multiple scenes
struct SceneState {
    int coinsCollected = 0;
};

struct Health {
    int currentHealth{};
};

struct FollowEntity {
    Entity followedEntity{};
    float xOffset{};
    float yOffset{};
};

struct KeyboardInputs {
    bool isHoldingLeft = false;
    bool isHoldingRight = false;
    bool isHoldingUp = false;
    bool isHoldingDown = false;
    bool isHoldingJump = false;
    bool isHoldingAttack = false;
};

struct ProjectileStats {
    float projectileSpeed{};
    Sprite sprite{};
    Vector2D direction{};
    Vector2D spawnPoint{};
    std::function<void(ProjectileStats)> spawnCallback{};
};

struct ProjectileLimit {
    int maxProjectiles{};
    int currentProjectiles{};
};

struct HasFired {
    bool fired{};
};

struct IsFiring {
    bool firing{};
    float firingDuration{};
    float startFiringDuration{};
    float endFiringDuration{};
    bool startFiring;
    bool endFiring;
    float timer{};
};

struct FiringPattern {
    Vector2D direction{};
    float interval{};
};

struct AutoFiring {
    float firingDelay{};
    std::vector<FiringPattern> patterns{};
    bool oneShot{};
    float timer{};
    int nextPattern{};
};


struct PlayerTag{};
struct PlayerGroundCheck{};
struct ProjectileTag{};
struct BeakEnemyTag{};

#endif //TUTORIAL1_COMPONENT_H