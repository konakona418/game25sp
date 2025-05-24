// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#include "Window.hpp"

#include "Common.hpp"
#include "Game.hpp"
#include "Logger.hpp"
#include "ThreadPool.hpp"
#include "systems/CollisionControl.hpp"
#include "systems/MovementControl.hpp"
#include "systems/MusicControl.hpp"
#include "systems/RenderControl.hpp"
#include "systems/SceneControl.hpp"
#include "systems/ScriptsControl.hpp"
#include "systems/TweeningControl.hpp"

namespace game {
    void Window::setVideoPreferences(const int fps, const bool vsync) {
        m_videoPreference = { fps, vsync };
        if (m_window != nullptr) {
            m_window->setFramerateLimit(fps);
            m_window->setVerticalSyncEnabled(vsync);
            getLogger().logDebug("Video preferences applied.");
        } else {
            getLogger().logDebug("Window not initialized, video preferences will be applied when window is created.");
        }
        getLogger().logInfo("Video preferences set to: " + std::to_string(fps) + " fps, " + (vsync ? "using" : "not using") + " vsync");
    }

    void Window::setWindowTitle(sf::String title) {
        m_windowTitle = std::move(title);
    }

    void Window::setWindowSize(const sf::Vector2u& windowSize) {
        m_windowSize = windowSize;
    }

    void Window::run() {
        m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_windowSize), m_windowTitle);
        setVideoPreferences(m_videoPreference.fps, m_videoPreference.vsync);

        auto& game = getGame();
        auto& keyboard = game.getKeyboard();

        sf::Clock internalClock;
        internalClock.start();

        while (m_window->isOpen()) {
            while (auto event = m_window->pollEvent()) {
                if (event.has_value()) {
                    if (event->is<sf::Event::Closed>()) {
                        m_window->close();
                        return;
                    }
                    if (event->is<sf::Event::Resized>()) {
                        keepViewportScale();
                    }
                    if (event->is<sf::Event::KeyPressed>()) {
                        keyboard.press(event->getIf<sf::Event::KeyPressed>()->code);
                    }
                    if (event->is<sf::Event::KeyReleased>()) {
                        keyboard.release(event->getIf<sf::Event::KeyReleased>()->code);
                    }
                }
            }

            auto deltaTime = internalClock.restart();
            const auto timeScale = game.getTimeScale();

            deltaTime *= timeScale;

            // DO NOT write the logic in event polling loop!!
            SScriptsSystem::update(deltaTime);

            SMovementSystem::update(deltaTime);
            SScenePositionUpdateSystem::update();
            SCollisionSystem::update(deltaTime);
            STweenSystem::update(deltaTime);

            SMusicSystem::update();

            m_window->clear();

            SRenderSystem::update(*m_window, deltaTime);
            m_window->display();

            SSceneUnmountSystem::update();
        }
    }

    // letterboxing code from:
    // https://github.com/SFML/SFML/wiki/Source%3A-Letterbox-effect-using-a-view
    sf::View getLetterboxView(sf::View view, sf::Vector2u windowSize) {

        // Compares the aspect ratio of the window to the aspect ratio of the view,
        // and sets the view's viewport accordingly in order to achieve a letterbox effect.
        // A new view (with a new viewport set) is returned.

        float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
        float viewRatio = view.getSize().x / view.getSize().y;
        float sizeX = 1;
        float sizeY = 1;
        float posX = 0;
        float posY = 0;

        bool horizontalSpacing = true;
        if (windowRatio < viewRatio)
            horizontalSpacing = false;

        // If horizontalSpacing is true, the black bars will appear on the left and right side.
        // Otherwise, the black bars will appear on the top and bottom.

        if (horizontalSpacing) {
            sizeX = viewRatio / windowRatio;
            posX = (1 - sizeX) / 2.f;
        }

        else {
            sizeY = windowRatio / viewRatio;
            posY = (1 - sizeY) / 2.f;
        }

        std::stringstream ss;
        ss << "Letterboxing viewport set, with: ";
        ss << "posX: " << posX << ", posY: " << posY << ", sizeX: " << sizeX << ", sizeY: " << sizeY;
        getLogger().logDebug(ss.str());

        view.setViewport( sf::FloatRect({posX, posY}, {sizeX, sizeY}) );

        return view;
    }

    void Window::keepViewportScale() const {
        auto view = m_window->getView();
        auto windowSize = m_window->getSize();

        m_window->setView(getLetterboxView(view, windowSize));
    }
} // game
