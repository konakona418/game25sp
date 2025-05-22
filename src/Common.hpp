// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#ifndef COMMON_HPP
#define COMMON_HPP
#include <entt/entity/registry.hpp>

#include "SFML/System/Vector2.hpp"

namespace game {
    class Game;
    class Logger;
    class ThreadPool;
    struct ResourceManager;

    Game& getGame();
    Logger& getLogger();
    ThreadPool& getThreadPool();

    entt::registry& getRegistry();
    ResourceManager& getResourceManager();

    float random(float min, float max);
    sf::Vector2f random(sf::Vector2f min, sf::Vector2f max);
    sf::Vector2f random(sf::Vector2f max);
    bool randomBool();
    bool randomBool(float chance);
}

#endif //COMMON_HPP
