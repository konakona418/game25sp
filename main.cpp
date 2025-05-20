#include <iostream>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "Prelude.hpp"
#include "components/Render.hpp"
#include "components/Scripts.hpp"
#include "systems/MovementControl.hpp"

#include "test/SceneTreeTests.hpp"
#include "utils/AnimatedTextureGenerator.hpp"

auto fn(entt::entity entity, sf::Time deltaTime) -> void {
    auto& registry = game::getRegistry();
    auto& keyboard = game::getGame().getKeyboard();

    sf::Vector2f velocity = sf::Vector2f {0, 0};
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

    if (velocity.x != 0 || velocity.y != 0) {
        game::MovementHelper::move(entity, velocity * 400.0f * deltaTime.asSeconds());
    }
};

entt::entity create(size_t renderLayer) {
    auto& registry = game::getRegistry();
    auto entity = registry.create();
    game::SLayoutManagementSystem::builder()
        .setLocalPosition({100.0, 100.0})
        .setSize({32, 48})
        .setScale({3.0, 3.0})
        .setAnchor(game::CLayout::Anchor::MiddleCenter())
        .build(entity);
    game::SScenePositionManagementSystem::attachSceneTreeComponents(entity);

    registry.emplace<game::CRenderComponent>(entity);
    registry.emplace<game::CRenderLayerComponent>(entity, 10 - renderLayer);
    auto animatedTexture = game::AnimatedTextureGenerator()
        .setOffset(sf::Vector2f {0, 0})
        .setPlacement(sf::Vector2u {1, 6})
        .setSize(sf::Vector2f { 32, 48 })
        .setDuration(sf::seconds(0.15))
        .generate("idle", "idle.png");
    registry.emplace<game::CAnimatedSpriteRenderComponent>(entity, animatedTexture, true);

    entt::delegate<void(entt::entity, sf::Time)> delegate {};

    delegate.connect<&fn>();
    registry.emplace<game::CScriptsComponent>(entity, delegate);

    return entity;
}

int main() {
    game::Game& game = game::Game::createGame();
    //testSceneTree();
    game.setConfig({.fps = 60, .vsync = false});

    game::ResourceManager::getRawTextureCache()
        .load(entt::hashed_string {"test"}, "./test.jpg");
    std::optional<entt::entity> lastEntity;
    for (int i = 0; i < 1; i++) {
        auto e = create(i);
        if (lastEntity) {
            game::SScenePositionManagementSystem::attachChild(*lastEntity, e);
        }
        lastEntity = e;
    }

    game.run();

}
