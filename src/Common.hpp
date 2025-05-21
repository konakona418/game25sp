// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#ifndef COMMON_HPP
#define COMMON_HPP
#include <entt/entity/registry.hpp>

namespace game {
    class Game;
    class Logger;
    class ThreadPool;
    struct ResourceManager;

    extern Game& getGame();
    extern Logger& getLogger();
    extern ThreadPool& getThreadPool();

    extern entt::registry& getRegistry();
    extern ResourceManager& getResourceManager();
}

#endif //COMMON_HPP
