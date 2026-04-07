//
// Created by antho on 2026-03-29.
//

#include "SpawnPlayer.h"

#include "Game.h"
#include "manager/AssetManager.h"

void SpawnPlayer::spawn(World& world) {
    auto& player(world.createEntity());

    Vector2D spawnPosition = Vector2D(50, 6300);
    if (Game::gameState.currentCheckpoint < world.getMap().checkPointSpawnPoints.size()) {
        spawnPosition = world.getMap().checkPointSpawnPoints[Game::gameState.currentCheckpoint];
    }
    auto& playerTransform = player.addComponent<Transform>(spawnPosition, 0.0f, 1.0f);
    player.addComponent<Velocity>(Vector2D(0.0f,1.0f), 0.0f, 0.0f);
    player.addComponent<PlayerHorizontalMovement>(250.0f, 1500.0f, 1250.0f, 8.0f, 0.125f, false);
    player.addComponent<Gravity>(2400.0f, 2400.0f, true);
    player.addComponent<Jump>(860.0f, 9600.0f);
    player.addComponent<CoyoteTime>(false, 0.05f);

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture* tex = TextureManager::load("../Assets/Animations/megaman_anim.png");
    // SDL_FRect playerSrc {0, 0, 32, 44};
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst {playerTransform.position.x, playerTransform.position.y, 96, 96};

    player.addComponent<Sprite>(tex, playerSrc, playerDst);

    auto& playerCollider = player.addComponent<Collider>("Player");
    playerCollider.rect.w = playerDst.w - 62;
    playerCollider.rect.h = playerDst.h - 24;
    playerCollider.xOffset = 31;
    playerCollider.yOffset = 24;

    player.addComponent<PlayerTag>();
    player.addComponent<Health>(Game::gameState.playerHealth);

    player.addComponent<IsGrounded>(false);

    player.addComponent<LadderClimbing>(150.0f);
    player.addComponent<KeyboardInputs>();
    player.addComponent<IsFacingRight>();

    SDL_Texture* playerProjectileTex = TextureManager::load("../Assets/megaman_projectile.png");
    SDL_FRect playerProjectileSrc{0, 0, 8, 8};
    SDL_FRect playerProjectileDest{0, 0, 8 * 3, 8 * 3};
    Sprite newProjectileSprite {playerProjectileTex, playerProjectileSrc, playerProjectileDest};

    player.addComponent<ProjectileStats>(800.0f, 1, Vector2D(0, 0), Vector2D(0, 0),
        [&world, newProjectileSprite](ProjectileStats stats) {
            auto& projectile = world.createDeferredEntity();
            projectile.addComponent<Transform>(stats.spawnPoint, 0.0f, 1.0f);
            projectile.addComponent<Velocity>(stats.direction, stats.projectileSpeed);

            auto& projectileSprite = projectile.addComponent<Sprite>(newProjectileSprite);
            auto& projectileCollider = projectile.addComponent<Collider>("Player");
            projectileCollider.rect.w = projectileSprite.dst.w;
            projectileCollider.rect.h = projectileSprite.dst.h;
            projectile.addComponent<ProjectileDamage>(stats.damage);

            projectile.addComponent<PlayerTag>();
            projectile.addComponent<ProjectileTag>();
            projectile.addComponent<DestroyOutOfViewTag>();
            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("megamanBuster"));
        }
    );
    player.addComponent<ProjectileLimit>(3, 0);
    player.addComponent<IsFiring>(false, 0.2f);
    player.addComponent<HasFired>(false);
    player.addComponent<Invulnerability>(false);
    player.addComponent<InvulnerabilityTimer>(2.0f);
    player.addComponent<HitKnockback>(80.0f, 0.6f);
    player.addComponent<FlashWhileInvulnerable>(0.05f);

    auto& playerHurtbox(world.createEntity());
    auto& playerHurtboxCollider = playerHurtbox.addComponent<Collider>("Player");
    playerHurtboxCollider.rect.w = 26;
    playerHurtboxCollider.rect.h = 30;
    playerHurtbox.addComponent<PlayerHurtbox>();
    playerHurtbox.addComponent<Transform>(playerTransform);
    playerHurtbox.addComponent<FollowEntity>(&player, (96.0f - 26.0f) / 2, (96.0f - 30.0f) / 2);

    auto& playerGroundCheck (world.createEntity());
    auto& playerGroundCheckCollider = playerGroundCheck.addComponent<Collider>("Player");
    playerGroundCheckCollider.rect.w = playerCollider.rect.w;
    playerGroundCheckCollider.rect.h = 16.0f;
    playerGroundCheck.addComponent<PlayerGroundCheck>();
    playerGroundCheck.addComponent<Transform>(playerTransform);
    playerGroundCheck.addComponent<FollowEntity>(&player, playerCollider.xOffset, playerCollider.rect.h + 24.0f);

    std::vector newChildren = {&playerHurtbox, &playerGroundCheck};
    player.addComponent<Children>(newChildren);

    player.addComponent<OnDeathCallback>([&world](Entity* player) {
        Game::gameState.lives--;
        std::cout << "Lives: " << Game::gameState.lives << std::endl;
        auto& transition (world.createEntity());
        transition.addComponent<SceneTransitionDelay>(2.25f, "cutman");
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("megamanDefeat"));
        Game::checkSceneState();
    });

    player.addComponent<OnHitCallback>([](Entity* player, Entity* other) {
        auto& invulnerability = player->getComponent<Invulnerability>();
        auto& invulTimer = player->getComponent<InvulnerabilityTimer>();

        auto& hitKnockback = player->getComponent<HitKnockback>();
        auto& velocity = other->getComponent<Velocity>();
        auto& playerCollider = player->getComponent<Collider>().rect;
        auto& enemyCollider = other->getComponent<Collider>().rect;
        hitKnockback.isHitKnockback = true;

        if (other->hasComponent<ContactDamage>()) {
            float leftPenetrationDepth = playerCollider.x - (enemyCollider.x + enemyCollider.w);
            float rightPenetrationDepth = (playerCollider.x + playerCollider.w) - enemyCollider.x;
            if (std::abs(leftPenetrationDepth) < std::abs(rightPenetrationDepth)) {
                hitKnockback.isRightHit = false;
            }
            else {
                hitKnockback.isRightHit = true;
            }
        }

        else if (other->hasComponent<ProjectileDamage>()) {
            if (velocity.direction.x < 0) {
                hitKnockback.isRightHit = true;
            }
            else {
                hitKnockback.isRightHit = false;
            }
        }

        hitKnockback.timer = hitKnockback.minKnockbackTime;
        invulTimer.timer = invulTimer.invulnerabilityTime;
        invulnerability.isInvulnerable = true;
    });
}
