// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#include "Layout.hpp"
#include "Common.hpp"
#include "Logger.hpp"
#include "systems/SceneControl.hpp"

void game::SLayoutManagementSystem::Builder::build(const entt::entity& entity) const {
    auto& registry = game::getRegistry();

    if (!registry.any_of<CHasLayout>(entity)) {
        getLogger().logDebug("Entity does not have layout components, adding them");
        attachLayoutComponents(entity);
    }

    registry.get<CLayout>(entity).setAnchor(m_anchor);
    registry.get<CLayout>(entity).setLayoutType(m_layoutType);
    registry.get<CLocalTransform>(entity).setSize(m_size);
    registry.get<CLocalTransform>(entity).setPosition(m_localPos);
    registry.get<CLocalTransform>(entity).setScale(m_scale);
    registry.get<CGlobalTransform>(entity).setPosition(m_globalPos);
}

entt::entity game::SLayoutManagementSystem::attachLayoutComponents(entt::entity entity) {
    auto& registry = game::getRegistry();

    if (registry.any_of<CHasLayout>(entity)) {
        throw std::runtime_error("Entity already has layout components");
    }

    registry.emplace<CHasLayout>(entity);
    registry.emplace<CLayout>(entity);
    registry.emplace<CGlobalTransform>(entity);
    registry.emplace<CLocalTransform>(entity);
    return entity;
}

void game::SLayoutManagementSystem::setPosition(entt::entity entity, sf::Vector2f position) {
    auto& registry = game::getRegistry();
    registry.get<CLocalTransform>(entity).setPosition(position);
    SScenePositionUpdateSystem::markEntityAsDirty(entity);
}

void game::SLayoutManagementSystem::setAbsolutePosition(entt::entity entity, sf::Vector2f position) {
    auto& registry = game::getRegistry();
    registry.get<CGlobalTransform>(entity).setPosition(position);
    SScenePositionUpdateSystem::markEntityAsDirty(entity);
}

void game::SLayoutManagementSystem::setSize(entt::entity entity, sf::Vector2f size) {
    auto& registry = game::getRegistry();
    registry.get<CLocalTransform>(entity).setSize(size);
    SScenePositionUpdateSystem::markEntityAsDirty(entity);
}

void game::SLayoutManagementSystem::setAnchor(entt::entity entity, CLayout::Anchor anchor) {
    auto& registry = game::getRegistry();
    registry.get<CLayout>(entity).setAnchor(anchor);
    SScenePositionUpdateSystem::markEntityAsDirty(entity);
}

void game::SLayoutManagementSystem::setLayoutType(entt::entity entity, CLayout::LayoutType layoutType) {
    auto& registry = game::getRegistry();
    registry.get<CLayout>(entity).setLayoutType(layoutType);
    SScenePositionUpdateSystem::markEntityAsDirty(entity);
}
