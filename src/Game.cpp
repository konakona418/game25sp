// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#include "Game.hpp"

#include "Common.hpp"
#include "Logger.hpp"
#include "ThreadPool.hpp"
#include "Window.hpp"

game::Game::Game() {
    m_hardwareConcurrency = std::thread::hardware_concurrency();
}

void game::Game::init() {
    auto& ctx = m_registry.ctx();

    // my advice is, do not write log in the destructor of components,
    // as it can be pretty random - at least unknown - when logger will be destroyed.
    ctx.emplace<Logger>(true);
    auto& logger = m_registry.ctx().get<Logger>();
    logger.run();
    logger.logInfo("Logger up and running!");

    logger.logInfo("Initializing game");

    logger.logDebug("Current hardware concurrency: " + std::to_string(m_hardwareConcurrency));

    // one reserved for the render thread,
    // which is basically the main thread. another for the main thread.
    ctx.emplace<ThreadPool>(m_hardwareConcurrency - 2);
    auto& threadPool = m_registry.ctx().get<ThreadPool>();
    threadPool.run();

    ctx.emplace<ResourceManager>();

    ctx.emplace<Window>();
    auto& window = m_registry.ctx().get<Window>();
}

void game::Game::cleanup() {
    auto& ctx = m_registry.ctx();

    ctx.erase<Window>();
    ctx.erase<ThreadPool>();

    getLogger().logInfo("Closing logger");
    ctx.erase<Logger>();
}

void game::Game::runAsyncImpl(std::function<void()> fn) {
    getThreadPool().schedule(std::move(fn));
}

game::Game& game::Game::createGame() {
    if (!s_gameInitialized) {
        s_gameInitialized = true;
        g_gameInstance = std::unique_ptr<Game>(new Game);
        g_gameInstance->init();
    }
    return *g_gameInstance;
}

game::Game& game::Game::getInstance() { return *g_gameInstance; }

bool game::Game::isInitialized() { return s_gameInitialized; }

game::Window& game::Game::getWindow() {
    return m_registry.ctx().get<Window>();
}

bool game::Game::isRunning() const { return m_isRunning; }

// should be invoked only once in each loop.
sf::Time game::Game::getDeltaTime() {
    static sf::Clock clock;
    return clock.restart();
}

game::Game::~Game() {
    getLogger().logInfo("Closing game");
    cleanup();
}

void game::Game::run() {
    getLogger().logInfo("Starting game");
    runBlocking([this]() -> void {
        getWindow().run();
    });
}

entt::registry& game::Game::getRegistry() {
    return m_registry;
}

game::Logger& game::Game::getLogger() {
    return m_registry.ctx().get<Logger>();
}

game::ThreadPool& game::Game::getThreadPool() {
    return m_registry.ctx().get<ThreadPool>();
}

game::ResourceManager& game::Game::getResourceManager() {
    return m_registry.ctx().get<ResourceManager>();
}
