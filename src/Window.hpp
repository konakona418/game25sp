// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#ifndef WINDOW_H
#define WINDOW_H

#include <utility>

#include "SFML/Graphics.hpp"

namespace game {
    class Window {
    public:
        Window() : m_windowSize(800, 600), m_aspectRatio(800.f / 600.f) {};

        explicit Window(const sf::Vector2u& windowSize)
            : m_windowSize(windowSize), m_aspectRatio(static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y)) {}

        Window(const sf::Vector2u& windowSize, sf::String title)
            : m_windowSize(windowSize), m_aspectRatio(static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y)), m_windowTitle(std::move(title)) {}

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        ~Window() = default;

        void setVideoPreferences(int fps, bool vsync);

        void setWindowTitle(sf::String title);

        void setWindowSize(const sf::Vector2u& windowSize);

        void run();

    private:
        struct VideoPreference {
            int fps = 60;
            bool vsync = false;
        };

        std::unique_ptr<sf::RenderWindow> m_window { nullptr };
        sf::Vector2u m_windowSize;
        sf::String m_windowTitle = u8"Game";
        float m_aspectRatio { 0 };
        VideoPreference m_videoPreference;

        void keepViewportScale() const;
    };
} // game

#endif //WINDOW_H
