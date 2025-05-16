#include <iostream>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "Prelude.hpp"
#include "components/Render.hpp"

#include "test/SceneTreeTests.hpp"

int main() {
    game::Game& game = game::Game::createGame();
    //testSceneTree();
    auto& registry = game::getRegistry();
    auto entity = registry.create();
    game::SLayoutManagementSystem::builder()
        .setLocalPosition({150.0, 100.0})
        .setSize({200, 200.0})
        .setScale({2, 1})
        .build(entity);
    game::SScenePositionManagementSystem::attachSceneTreeComponents(entity);
    registry.emplace<game::CRenderComponent>(entity);

    auto texture =
        game::ResourceManager::getTextureCache()
        .load(entt::hashed_string {"test"}, "./test.jpg", sf::IntRect{{0, 10}, {200, 200}}).first->second;
    texture->texture.setRepeated(true);
    game::SpriteFrame spriteFrame(texture);
    registry.emplace<game::CSpriteRenderComponent>(entity, spriteFrame);

    game.run();

}
