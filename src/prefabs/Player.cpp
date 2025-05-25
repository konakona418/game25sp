// Game - NWPU C++ sp25
// Created on 2025/5/21
// by konakona418 (https://github.com/konakona418)

#include "Player.hpp"

#include "Bullet.hpp"
#include "Game.hpp"
#include "components/Collision.hpp"
#include "components/Layout.hpp"
#include "components/Scripts.hpp"
#include "components/Lighting.hpp"
#include "systems/CollisionControl.hpp"
#include "systems/MovementControl.hpp"
#include "systems/SceneControl.hpp"
#include "utils/TextureGenerator.hpp"
#include "utils/LazyLoader.hpp"
#include "utils/MovementUtils.hpp"
#include "Window.hpp"

void game::prefab::Player::onUpdate(entt::entity entity, sf::Time deltaTime) {
    auto& registry = game::getRegistry();
    auto& game = game::getGame();
    auto& keyboard = game.getKeyboard();

    sf::Vector2f velocity = sf::Vector2f{0, 0};
    if (keyboard.isKeyPressed(sf::Keyboard::Key::Up)) {
        velocity.y = -1;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Key::Down)) {
        velocity.y = 1;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -1;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = 1;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Key::LShift)) {
        game.setTimeScale(0.5f);
    } else {
        game.setTimeScale(1.0f);
    }

    auto& player = registry.get<game::prefab::GPlayerComponent>(entity);
    if (velocity.x < 0) {
        player.flipH = true;
    }
    else if (velocity.x > 0) {
        player.flipH = false;
    }

    auto& animatedSprite = registry.get<game::CAnimatedSpriteRenderComponent>(entity);
    if (velocity.lengthSquared() > 0) {
        animatedSprite.setFrames(player.animations["walk"]);
    }
    else {
        animatedSprite.setFrames(player.animations["idle"]);
    }

    game::MovementUtils::flipHorizontal(entity, player.flipH);

    constexpr float X_LIM = 1024.0;
    constexpr float Y_LIM = 1024.0;
    constexpr float clamping = 0.01f;

    auto& localTransform = registry.get<game::CLocalTransform>(entity);
    auto position = localTransform.getPosition();
    if (velocity.x != 0 || velocity.y != 0) {
        if (position.x < X_LIM && position.x > -X_LIM && position.y < Y_LIM && position.y > -Y_LIM) {
            game::MovementUtils::move(entity, velocity * 400.0f * deltaTime.asSeconds());
        } else {
            game::MovementUtils::move(entity, velocity * 100.0f * deltaTime.asSeconds());
            localTransform.setPosition({std::clamp(position.x, -X_LIM + clamping, X_LIM - clamping), std::clamp(position.y, -Y_LIM + clamping, Y_LIM - clamping)});
        }
    }

    auto& window = getGame().getWindow();
    auto& globalTransform = registry.get<game::CGlobalTransform>(entity);
    window.setViewCenter(globalTransform.getPosition());
};

void game::prefab::Player::onCollision(game::EOnCollisionEvent e) {
    auto& registry = game::getRegistry();
    auto entity1 = e.collider1;
    auto entity2 = e.collider2;

    auto pair = game::which<
        game::prefab::GPlayerComponent,
        game::prefab::GBulletComponent>(entity1, entity2);
    if (pair) {
        auto& playerComponent = registry.get<game::prefab::GPlayerComponent>(pair->first);
        game::SceneTreeUtils::unmount(pair->second);
        playerComponent.health -= 10;
        game::getLogger().logDebug("Player health: " + std::to_string(playerComponent.health));

        game::getEventDispatcher().trigger<EOnPlayerDamageEvent>({ pair->first, playerComponent.health });

        if (playerComponent.health <= 0) {
            game::SceneTreeUtils::unmount(pair->first);
            game::getLogger().logInfo("Player died!");
            game::getEventDispatcher().trigger<EOnPlayerDeathEvent>({ pair->first });
        }
    }
}

game::prefab::Player game::prefab::Player::create() {
    static entt::connection collisionConn;
    collisionConn = game::getEventDispatcher().sink<game::EOnCollisionEvent>().connect<&onCollision>();
    return {};
}

game::prefab::Player::Player() : TreeLike() {
    auto& registry = game::getRegistry();
    auto entity = registry.create();
    m_entity = entity;

    game::MovementUtils::builder()
        .setLocalPosition({0.0, 0.0})
        .setSize({32, 48})
        .setScale({3.0, 3.0})
        .setAnchor(game::CLayout::Anchor::MiddleCenter())
        .build(entity);
    game::SceneTreeUtils::attachSceneTreeComponents(entity);

    registry.emplace<game::CRenderComponent>(entity);
    registry.emplace<game::CRenderLayerComponent>(entity, RENDER_LAYER, 0);
    registry.emplace<game::CRenderTargetComponent>(entity, game::CRenderTargetComponent::GameComponent);

    auto animations = loadAnimationResources();
    registry.emplace<game::CAnimatedSpriteRenderComponent>(entity, animations["idle"], true);
    entt::delegate<void(entt::entity, sf::Time)> delegate{};

    delegate.connect<&onUpdate>();
    registry.emplace<game::CScriptsComponent>(entity, delegate);

    registry.emplace<game::CCollisionComponent>(entity);
    registry.emplace<game::CCollisionAABBComponent>(entity, sf::Vector2f {32.f, 48.f});
    // on layer 1, collide with enemy bullet(2)
    registry.emplace<game::CCollisionLayerComponent>(entity,
        CollisionUtils::getCollisionMask(1), CollisionUtils::getCollisionMask(2));

    registry.emplace<game::CLightingComponent>(entity, sf::Color(255, 0, 255, 196), 100.f);

    registry.emplace<game::prefab::GPlayerComponent>(entity, animations);
}

std::unordered_map<std::string, entt::resource<game::AnimatedFrames>> game::prefab::Player::loadAnimationResources() {
    static game::Lazy<std::unordered_map<std::string, entt::resource<AnimatedFrames>>> animations {
        [] {
            std::unordered_map<std::string, entt::resource<AnimatedFrames>> res;
            res.emplace("idle", ResourceManager::getAnimatedFramesCache()
                .load(entt::hashed_string {"playerIdleAnimation"},
                    game::AnimatedTextureGenerator()
                        .setOffset(sf::Vector2f{0, 0})
                        .setPlacement(sf::Vector2u{1, 6})
                        .setSize(sf::Vector2f{32, 48})
                        .setDuration(sf::seconds(0.1))
                        .generate("playerIdle", "assets/idle.png")).first->second);
            res.emplace("walk", ResourceManager::getAnimatedFramesCache()
                .load(entt::hashed_string {"playerWalkAnimation"},
                    game::AnimatedTextureGenerator()
                        .setOffset(sf::Vector2f{0, 0})
                        .setPlacement(sf::Vector2u{1, 8})
                        .setSize(sf::Vector2f{32, 48})
                        .setDuration(sf::seconds(0.1))
                        .generate("playerWalk", "assets/walk.png")).first->second);
            return res;
        }
    };

    return *animations;
}
