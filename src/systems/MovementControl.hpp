// Game - NWPU C++ sp25
// Created on 2025/5/20
// by konakona418 (https://github.com/konakona418)

#ifndef MOVEMENTCONTROL_HPP
#define MOVEMENTCONTROL_HPP
#include <entt/entity/entity.hpp>

#include "components/Layout.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

    class MovementHelper {
    public:
        static void markAsDirty(entt::entity entity);
        static void markAsClean(entt::entity entity);

        static void setPosition(entt::entity entity, sf::Vector2f position);
        static void setSize(entt::entity entity, sf::Vector2f size);
        static void setScale(entt::entity entity, sf::Vector2f scale);

        static void setAnchor(entt::entity entity, CLayout::Anchor anchor);
        static void setLayoutType(entt::entity entity, CLayout::LayoutType layoutType);

        static void move(entt::entity entity, sf::Vector2f offset);
    };

} // game

#endif //MOVEMENTCONTROL_HPP
