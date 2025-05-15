// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#ifndef GAME_HPP
#define GAME_HPP

#include <entt/entt.hpp>

namespace game {
    class Logger;
    class ThreadPool;
    class Window;

    class Game {
    public:
        static Game& createGame();

        ~Game();

        void run();

        entt::registry& getRegistry();

        Logger& getLogger();

        ThreadPool& getThreadPool();

        static Game& getInstance();

        static bool isInitialized();

        Window& getWindow();

        [[nodiscard]] bool isRunning() const;

        template <typename Fn, std::enable_if_t<std::is_invocable_v<Fn>, int> = 0>
        static void runBlocking(Fn fn) {
            fn();
        }

        template <typename Fn, std::enable_if_t<std::is_invocable_v<Fn>, int> = 0>
        static void runAsync(Fn fn) {
            static_assert(std::is_convertible_v<Fn, std::function<void()>> ||
                std::is_same_v<Fn, std::function<void()>>, "Fn must be convertible to std::function<void()> or std::function<void()>.");
            if constexpr (std::is_same_v<Fn, std::function<void()>>) {
                getInstance().runAsyncImpl(fn);
            }
            getInstance().runAsyncImpl(std::function<void()>(fn));
        }

    private:
        entt::registry m_registry;
        uint32_t m_hardwareConcurrency;
        bool m_isRunning { false };

        Game(Game&& other) noexcept = default;
        Game& operator=(Game&& other) noexcept = default;

        Game();

        void init();
        void cleanup();
        void runAsyncImpl(std::function<void()> fn);
    };

    inline bool s_gameInitialized = false;
    inline extern std::unique_ptr<Game> g_gameInstance { nullptr };
}


#endif //GAME_HPP
