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
    game::SScenePositionManagementSystem::attachSceneTreeComponents(root);

    auto comp1 = registry.create();
    registry.emplace<game::CLocalTransform>(comp1, sf::Vector2f(100, 100));
    registry.emplace<game::CGlobalTransform>(comp1, sf::Vector2f(0, 0));
    registry.emplace<game::CLayout>(comp1, game::CLayout::Anchor::MiddleCenter(), game::CLayout::LayoutType::Relative);
    registry.get<game::CLocalTransform>(comp1).setSize({50, 50});

    game::SScenePositionManagementSystem::attachSceneTreeComponents(comp1);
    game::SScenePositionManagementSystem::attachChild(root, comp1);

    game::SScenePositionUpdateSystem::update();

    auto comp2 = registry.create();
    game::SLayoutManagementSystem::builder()
        .setLocalPosition(sf::Vector2f(10, 10))
        .setSize(sf::Vector2f(100, 100))
        .setAnchor(game::CLayout::Anchor::TopLeft())
        .build(comp2);
    game::SScenePositionManagementSystem::attachSceneTreeComponents(comp2);
    game::SScenePositionManagementSystem::attachChild(comp1, comp2);

    auto comp3 = registry.create();
    game::SLayoutManagementSystem::builder()
        .setLocalPosition(sf::Vector2f(20, 20))
        .setSize(sf::Vector2f(100, 100))
        .setAnchor(game::CLayout::Anchor::TopLeft())
        .build(comp3);
    game::SScenePositionManagementSystem::attachSceneTreeComponents(comp3);
    game::SScenePositionManagementSystem::attachChild(comp1, comp3);

    game::SScenePositionUpdateSystem::update();

    assert(registry.get<game::CGlobalTransform>(root).getPosition() == sf::Vector2f(50, 50));
    assert(registry.get<game::CGlobalTransform>(comp1).getPosition() == sf::Vector2f(125, 125));
    assert(registry.get<game::CGlobalTransform>(comp2).getPosition() == sf::Vector2f(135, 135));
    assert(registry.get<game::CGlobalTransform>(comp3).getPosition() == sf::Vector2f(145, 145));

    game::SLayoutManagementSystem::setPosition(comp1, sf::Vector2f(50, 50));
    game::SScenePositionUpdateSystem::update();

    assert(registry.get<game::CGlobalTransform>(root).getPosition() == sf::Vector2f(50, 50));
    assert(registry.get<game::CGlobalTransform>(comp1).getPosition() == sf::Vector2f(75, 75));
    assert(registry.get<game::CGlobalTransform>(comp2).getPosition() == sf::Vector2f(85, 85));
    assert(registry.get<game::CGlobalTransform>(comp3).getPosition() == sf::Vector2f(95, 95));

    game::SScenePositionManagementSystem::detachChild(comp1, comp2);
    game::SScenePositionManagementSystem::detachChild(comp1, comp3);
    game::SScenePositionUpdateSystem::update();
    assert(registry.get<game::CGlobalTransform>(root).getPosition() == sf::Vector2f(50, 50));
    assert(registry.get<game::CGlobalTransform>(comp1).getPosition() == sf::Vector2f(75, 75));
    assert(registry.get<game::CGlobalTransform>(comp2).getPosition() == sf::Vector2f(10, 10));
    assert(registry.get<game::CGlobalTransform>(comp3).getPosition() == sf::Vector2f(20, 20));
}

#endif //SCENETREETESTS_HPP
