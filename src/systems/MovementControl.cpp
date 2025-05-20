// Game - NWPU C++ sp25
// Created on 2025/5/20
// by konakona418 (https://github.com/konakona418)

#include "MovementControl.hpp"

#include "SceneControl.hpp"

namespace game {
    void MovementHelper::markAsDirty(entt::entity entity) {
        if (SScenePositionManagementSystem::isDirty(entity)) {
            return;
        }
        SScenePositionManagementSystem::markAsDirty(entity);
    }

    void MovementHelper::markAsClean(entt::entity entity) {
        if (!SScenePositionManagementSystem::isDirty(entity)) {
            return;
        }
        SScenePositionManagementSystem::markAsClean(entity);
    }

    void MovementHelper::setPosition(entt::entity entity, sf::Vector2f position) {
        auto& registry = getRegistry();
        registry.patch<CLocalTransform>(entity, [position](CLocalTransform& localTransform) {
            localTransform.setPosition(position);
        });
        markAsDirty(entity);
    }

    void MovementHelper::setSize(entt::entity entity, sf::Vector2f size) {
        auto& registry = getRegistry();
        registry.patch<CLocalTransform>(entity, [size](CLocalTransform& localTransform) {
            localTransform.setSize(size);
        });
        markAsDirty(entity);
    }

    void MovementHelper::setScale(entt::entity entity, sf::Vector2f scale) {
        auto& registry = getRegistry();
        registry.patch<CLocalTransform>(entity, [scale](CLocalTransform& localTransform) {
            localTransform.setScale(scale);
        });
        markAsDirty(entity);
    }

    void MovementHelper::setAnchor(entt::entity entity, CLayout::Anchor anchor) {
        auto& registry = getRegistry();
        registry.patch<CLayout>(entity, [anchor](CLayout& layout) {
            layout.setAnchor(anchor);
        });
    }

    void MovementHelper::setLayoutType(entt::entity entity, CLayout::LayoutType layoutType) {
        auto& registry = getRegistry();
        registry.patch<CLayout>(entity, [layoutType](CLayout& layout) {
            layout.setLayoutType(layoutType);
        });
        markAsDirty(entity);
    }

    void MovementHelper::move(entt::entity entity, sf::Vector2f offset) {
        auto& registry = getRegistry();
        registry.patch<CLocalTransform>(entity, [offset](CLocalTransform& localTransform) {
            localTransform.move(offset);
        });
        markAsDirty(entity);
    }
} // game