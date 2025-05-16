// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#include "Window.hpp"

#include "systems/RenderControl.hpp"
#include "systems/SceneControl.hpp"

namespace game {
    void Window::setPreferences(const int fps, const bool vsync) const {
        if (m_window != nullptr) {
            m_window->setFramerateLimit(fps);
            m_window->setVerticalSyncEnabled(vsync);
        }
    }

    void Window::run() {
        m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_windowSize), m_windowTitle);
        while (m_window->isOpen()) {
            while (auto event = m_window->pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    m_window->close();
                    return;
                }
                if (event->is<sf::Event::Resized>()) {
                    keepViewportScale();
                }
                m_window->clear();
                SScenePositionUpdateSystem::update();
                SRenderSystem::update(*m_window);
                m_window->display();
            }
        }
    }

    void Window::keepViewportScale() {}
} // game
