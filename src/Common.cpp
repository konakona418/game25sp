// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#include "Common.hpp"

#include "Game.hpp"

namespace game {
    Game& getGame() {
        return Game::getInstance();
    }

    extern Logger& getLogger() {
        return getGame().getLogger();
    }

    extern ThreadPool& getThreadPool() {
        return getGame().getThreadPool();
    }

    extern entt::registry& getRegistry() {
        return getGame().getRegistry();
    }
}
