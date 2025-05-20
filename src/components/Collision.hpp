// Game - NWPU C++ sp25
// Created on 2025/5/20
// by konakona418 (https://github.com/konakona418)

#ifndef COLLISION_HPP
#define COLLISION_HPP
#include "SFML/Graphics/Rect.hpp"

namespace game {
    struct CCollisionComponent {};

    struct CCollisionAABBComponent {
        explicit CCollisionAABBComponent(const sf::Vector2f& size) : m_boundingBoxSize(size) {}

        [[nodiscard]] sf::Vector2f getBoundingBox() const { return m_boundingBoxSize; }
        void setBoundingBox(sf::Vector2f size) { m_boundingBoxSize = size; }
    private:
        sf::Vector2f m_boundingBoxSize;
    };

    struct CCollisionCircleComponent {
        explicit CCollisionCircleComponent(float radius) : m_radius(radius) {}

        [[nodiscard]] float getRadius() const { return m_radius; }
        void setRadius(float radius) { m_radius = radius; }
    private:
        float m_radius;
    };

} // game

#endif //COLLISION_HPP
