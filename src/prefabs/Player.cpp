// Game - NWPU C++ sp25
// Created on 2025/5/21
// by konakona418 (https://github.com/konakona418)

#include "Player.hpp"

#include "Game.hpp"
#include "components/Layout.hpp"
#include "components/Scripts.hpp"
#include "systems/MovementControl.hpp"
#include "systems/SceneControl.hpp"
#include "utils/AnimatedTextureGenerator.hpp"
#include "utils/MovementUtils.hpp"

auto onKeyPress(entt::entity entity, sf::Time deltaTime) -> void {
    auto& registry = game::getRegistry();
    auto& keyboard = game::getGame().getKeyboard();

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

    if (velocity.x != 0 || velocity.y != 0) {
        game::MovementUtils::move(entity, velocity * 400.0f * deltaTime.asSeconds());
    }
};

game::prefab::Player game::prefab::Player::create() {
    return {};
}

void game::prefab::Player::detach() const {
    SceneTreeUtils::unmount(m_entity);
}

game::prefab::Player::Player() {
    auto& registry = game::getRegistry();
    auto entity = registry.create();
    m_entity = entity;

    game::MovementUtils::builder()
        .setLocalPosition({100.0, 100.0})
        .setSize({32, 48})
        .setScale({3.0, 3.0})
        .setAnchor(game::CLayout::Anchor::MiddleCenter())
        .build(entity);
    game::SceneTreeUtils::attachSceneTreeComponents(entity);

    registry.emplace<game::CRenderComponent>(entity);
    registry.emplace<game::CRenderLayerComponent>(entity, RENDER_LAYER);

    auto animations = loadAnimationResources();
    registry.emplace<game::CAnimatedSpriteRenderComponent>(entity, animations["idle"], true);
    entt::delegate<void(entt::entity, sf::Time)> delegate{};

    delegate.connect<&onKeyPress>();
    registry.emplace<game::CScriptsComponent>(entity, delegate);

    registry.emplace<game::prefab::GPlayerComponent>(entity, animations);
}

std::unordered_map<std::string, game::AnimatedFrames> game::prefab::Player::loadAnimationResources() {
    static Lazy<std::unordered_map<std::string, AnimatedFrames>> animations {
        [] {
            std::unordered_map<std::string, AnimatedFrames> res;
            res.emplace("idle",
                               game::AnimatedTextureGenerator()
                               .setOffset(sf::Vector2f{0, 0})
                               .setPlacement(sf::Vector2u{1, 6})
                               .setSize(sf::Vector2f{32, 48})
                               .setDuration(sf::seconds(0.1))
                               .generate("playerIdle", "idle.png"));
            res.emplace("walk",
                               game::AnimatedTextureGenerator()
                               .setOffset(sf::Vector2f{0, 0})
                               .setPlacement(sf::Vector2u{1, 8})
                               .setSize(sf::Vector2f{32, 48})
                               .setDuration(sf::seconds(0.1))
                               .generate("playerWalk", "walk.png"));
            return res;
        }
    };

    return *animations;
}
