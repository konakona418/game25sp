// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#include "Game.hpp"

#include "Logger.hpp"
#include "ThreadPool.hpp"
#include "Window.hpp"

game::Game::Game() {
    m_hardwareConcurrency = std::thread::hardware_concurrency();
}

void game::Game::init() {
    auto& ctx = m_registry.ctx();

    // 建议是不要在这些组件的析构里面写日志，因为不知道 logger 何时被析构。
    ctx.emplace<Logger>(true);
    auto& logger = m_registry.ctx().get<Logger>();
    logger.run();
    logger.logInfo("Logger up and running!");

    logger.logInfo("Initializing game");

    logger.logDebug("Current hardware concurrency: " + std::to_string(m_hardwareConcurrency));
    ctx.emplace<ThreadPool>(m_hardwareConcurrency - 2); // 一个留给渲染线程， 一个留给 logger。
    auto& threadPool = m_registry.ctx().get<ThreadPool>();
    threadPool.run();

    ctx.emplace<Window>();
    auto& window = m_registry.ctx().get<Window>();
}

void game::Game::cleanup() {
    m_registry.ctx().erase<ThreadPool>();

    getLogger().logInfo("Closing logger");
    m_registry.ctx().erase<Logger>();
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
