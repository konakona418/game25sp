// Game - NWPU C++ sp25
// Created on 2025/5/20
// by konakona418 (https://github.com/konakona418)

#ifndef COLLISIONCONTROL_HPP
#define COLLISIONCONTROL_HPP

#include <entt/entity/entity.hpp>

#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

    struct EOnCollisionEvent {
        entt::entity collider1;
        entt::entity collider2;
    };

    class SCollisionSystem {
    public:
        SCollisionSystem() = default;

        static void update(sf::Time deltaTime);
    private:
        static bool checkCollisionBoxes(entt::registry& reg, const entt::entity& entity1, const entt::entity& entity2);
        static bool checkCollisionCircles(entt::registry& reg, const entt::entity& entity1, const entt::entity& entity2);
        static bool checkCollisionBoxCircle(entt::registry& reg, const entt::entity& entity1, const entt::entity& entity2);
        static void emitSignal(entt::registry& reg, const entt::entity& entity1, const entt::entity& entity2);
        static std::pair<ssize_t, ssize_t> mapGrid(const sf::Vector2f& position, sf::Vector2f gridSize = {32.f, 32.f});
    };

} // game

#endif //COLLISIONCONTROL_HPP
