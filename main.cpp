#include <iostream>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "Prelude.hpp"
#include "components/Render.hpp"

#include "test/SceneTreeTests.hpp"

entt::entity create(size_t renderLayer) {
    auto& registry = game::getRegistry();
    auto entity = registry.create();
    game::SLayoutManagementSystem::builder()
        .setLocalPosition({100.0, 100.0})
        .setSize({200.0, 200.0})
        .setScale({1.0, 1.0})
        .setAnchor(game::CLayout::Anchor::MiddleCenter())
        .build(entity);
    game::SScenePositionManagementSystem::attachSceneTreeComponents(entity);

    registry.emplace<game::CRenderComponent>(entity);
    registry.emplace<game::CRenderLayerComponent>(entity, 10 - renderLayer);
    auto texture =
        game::ResourceManager::getTextureCache()
        .load(entt::hashed_string {"test"}, "./test.jpg",
            sf::IntRect{{0, 10}, {200, 200}}).first->second;
    texture->texture.setRepeated(true);
    game::SpriteFrame spriteFrame(texture);
    registry.emplace<game::CSpriteRenderComponent>(entity, spriteFrame);

    return entity;
}

int main() {
    game::Game& game = game::Game::createGame();
    //testSceneTree();

    std::optional<entt::entity> lastEntity;
    for (int i = 0; i < 10; i++) {
        auto e = create(i);
        if (lastEntity) {
            game::SScenePositionManagementSystem::attachChild(*lastEntity, e);
        }
        lastEntity = e;
    }

    game.run();

}
