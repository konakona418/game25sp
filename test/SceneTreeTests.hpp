// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#ifndef SCENETREETESTS_HPP
#define SCENETREETESTS_HPP

#include "components/Layout.hpp"
#include "systems/SceneControl.hpp"

inline void testSceneTree() {
    auto& registry = game::getRegistry();
    auto root = registry.create();
    game::SLayoutManagementSystem::builder()
        .setLocalPosition(sf::Vector2f(100, 100))
        .setSize(sf::Vector2f(100, 100))
        .setAnchor(game::CLayout::Anchor::MiddleCenter())
        .build(root);
    game::SSceneManagementSystem::attachSceneTreeComponents(root);

    auto comp1 = registry.create();
    registry.emplace<game::CLocalPosition>(comp1, sf::Vector2f(100, 100));
    registry.emplace<game::CGlobalPosition>(comp1, sf::Vector2f(0, 0));
    registry.emplace<game::CLayout>(comp1, game::CLayout::Anchor::MiddleCenter(), game::CLayout::LayoutType::Relative);
    registry.emplace<game::CSize>(comp1, sf::Vector2f(50, 50));

    game::SSceneManagementSystem::attachSceneTreeComponents(comp1);
    game::SSceneManagementSystem::attachChild(root, comp1);

    game::SScenePositionSystem::update();

    auto comp2 = registry.create();
    game::SLayoutManagementSystem::builder()
        .setLocalPosition(sf::Vector2f(10, 10))
        .setSize(sf::Vector2f(100, 100))
        .setAnchor(game::CLayout::Anchor::TopLeft())
        .build(comp2);
    game::SSceneManagementSystem::attachSceneTreeComponents(comp2);
    game::SSceneManagementSystem::attachChild(comp1, comp2);

    game::SScenePositionSystem::update();

    assert(registry.get<game::CGlobalPosition>(root).getPosition() == sf::Vector2f(50, 50));
    assert(registry.get<game::CGlobalPosition>(comp1).getPosition() == sf::Vector2f(125, 125));
    assert(registry.get<game::CGlobalPosition>(comp2).getPosition() == sf::Vector2f(135, 135));

    game::SLayoutManagementSystem::setPosition(comp1, sf::Vector2f(50, 50));
    game::SScenePositionSystem::update();

    assert(registry.get<game::CGlobalPosition>(root).getPosition() == sf::Vector2f(50, 50));
    assert(registry.get<game::CGlobalPosition>(comp1).getPosition() == sf::Vector2f(75, 75));
    assert(registry.get<game::CGlobalPosition>(comp2).getPosition() == sf::Vector2f(85, 85));

    game::SSceneManagementSystem::detachChild(comp1, comp2);
    game::SScenePositionSystem::update();
    assert(registry.get<game::CGlobalPosition>(root).getPosition() == sf::Vector2f(50, 50));
    assert(registry.get<game::CGlobalPosition>(comp1).getPosition() == sf::Vector2f(75, 75));
    assert(registry.get<game::CGlobalPosition>(comp2).getPosition() == sf::Vector2f(10, 10));
}

#endif //SCENETREETESTS_HPP
