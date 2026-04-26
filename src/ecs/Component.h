//
// Created by antho on 2026-01-21.
//

#ifndef TUTORIAL1_COMPONENT_H
#define TUTORIAL1_COMPONENT_H
#include <functional>
#include <memory>
#include <SDL3/SDL_render.h>

#include "Vector2D.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "system/Visuals/AnimationSystems/AnimationClip.h"
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

struct PlayerHorizontalMovement {
    float maxSpeed{};
    float acceleration{};
    float deacceleration{};
    float inchDistance{};
    float inchDuration{};
    bool isInching{};
    float timer{};
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

struct CoyoteTime {
    bool isCoyoteTime{};
    float duration{};
    float timer{};
};

enum class RenderLayer {
    World,
    UI
};

struct Sprite {
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
    RenderLayer renderLayer = RenderLayer::World;
    bool isVisible = true;
    bool normallyVisible = true;
};

struct Collider {
    std::string tag;
    float xOffset{};
    float yOffset{};
    SDL_FRect rect{};
};

struct WallCollidable {
    bool isCollidable{};
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
    float worldX;
    float worldY;
};

struct TimedSpawner {
    float spawnInterval{};
    std::function<void()> spawnCallback{};
    float timer{};
};

struct SpawnOnVisible {
    bool isVisible{};
    std::function<Entity*(Transform)> spawnCallback{};
    Entity* spawnedEntity = nullptr;
};

struct SpawnWhileVisibleTimer {
    float shortDelay{};
    float longDelay{};
    std::function<Entity*(Transform)> spawnCallback{};
    Entity* spawnedEntity = nullptr;
    float timer{};
};

// Our game state, might have multiple scenes
struct SceneState {
    int playerHealth{};
    int playerMaxHealth{};
    int lives{};
    int currentCheckpoint{};
    bool isEnding{};
    bool isPaused{};
};

struct Health {
    int currentHealth{};
    int maxHealth = currentHealth;
};

struct FollowParent {
    float xOffset{};
    float yOffset{};
};

struct Parent {
    Entity* parent{};
};

struct Children {
    std::vector<Entity*> children{};
};

struct MoveTowardsPlayer {
    bool isMovingTowards{};
    bool moveX{};
    bool moveY{};
    float xSpeed{};
    float ySpeed{};
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
    int damage{};
    Vector2D direction{};
    Vector2D spawnPoint{};
    std::function<void(ProjectileStats, Entity)> spawnCallback{};
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
    bool startFiring{};
    bool endFiring{};
    float timer{};
};

struct FiringPattern {
    Vector2D direction{};
    float interval{};
};

struct AutoFiring {
    float firingDelay{};
    std::vector<FiringPattern> patterns{};
    bool loop = true;
    bool burst = false;
    float timer{};
    int nextPattern{};
};

struct ContactDamage {
    int damage{};
};

struct ProjectileDamage {
    int damage{};
};

struct Damage {
    int damage{};
    Entity* damagedEntity{};
    Entity* damageDealerEntity{};
    bool invulIgnore{};
};

struct HealthBarUpdate {
    std::function<void(Entity*)> callback{};
};

struct OnHitCallback {
    std::function<void(Entity*, Entity*)> callback{};
};

struct OnDeathCallback {
    std::function<void(Entity*)> callback{};
};

struct OnPlayerDetectEnterCallback {
    std::function<void(Entity*, Entity*)> callback{};
};

struct OnPlayerDetectStayCallback {
    std::function<void(Entity*, Entity*)> callback{};
};

struct OnPlayerDetectLeaveCallback {
    std::function<void(Entity*, Entity*)> callback{};
};

struct BladerAttack {
    bool isAttacking{};
    float initialYPosition{};
    float totalAttackDuration{};
    float firstAttackDuration{};
    float firstPhaseDistanceFromPlayer{};
    float secondPhaseXDistance{};
    float xAcceleration{};
    float yAcceleration{};
    float timer{};
    bool firstPhase{};
    bool secondPhase{};
};

struct OctopusBatteryStats {
    float speed{};
    float waitDuration{};
    float timer{};
};

struct SceneTransitionDelay {
    float timer{};
    std::string nextScene{};
};

struct GameStarting {
    bool isStarting{};
};

struct GamePaused {
    bool hasPaused{};
};

struct Invulnerability {
    bool isInvulnerable{};
};

struct InvulnerabilityTimer {
    float invulnerabilityTime{};
    float timer{};
};

struct FlashTimer {
    float flashInterval{};
    float flashDuration{};
    float intervalTimer{};
    float durationTimer{};
};

struct HitKnockback {
    float knockbackSpeed{};
    float minKnockbackTime{};
    bool isHitKnockback{};
    bool isRightHit{}; // For knockback direction
    float timer{};
};

struct StopMovementWhileFiring {
    Vector2D direction{};
};

struct TitleScreenTag{};
struct PlayerTag{};
struct PlayerGroundCheck{};
struct PlayerHurtbox{};
struct PlayerHitFlash{};
struct PlayerHitParticles{};

struct ProjectileTag{};
struct BeakEnemyTag{};
struct BladerEnemyTag{};
struct OctopusBatteryTag{};
struct SuperCutterTag{};
struct FlyingShellEnemyTag{};
struct EnemyDeathTag{};

struct InvulnerableWhileNotFiring{};
struct SpawnedEnemyTag{};
struct DestroyOutOfViewTag{};

struct VictoryItemTag{};


#endif //TUTORIAL1_COMPONENT_H