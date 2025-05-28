// Game - NWPU C++ sp25
// Created on 2025/5/28
// by konakona418 (https://github.com/konakona418)

#ifndef GAME25SP_SPLASHSCREEN_HPP
#define GAME25SP_SPLASHSCREEN_HPP

#include "entt/resource/resource.hpp"
#include "components/Render.hpp"
#include "systems/SceneControl.hpp"

namespace game::prefab {

    struct EOnSplashScreenCompletedEvent {
        entt::entity entity;
    };

    class SplashScreen : game::TreeLike {
    public:
        static SplashScreen create() { return {}; };
    private:
        static constexpr float SPLASH_SCREEN_DURATION = 4.0f;

        SplashScreen();
        static entt::resource<game::SpriteFrame> loadImage();
        static void onTweenCompleted(entt::entity entity);

    };

} // game

#endif //GAME25SP_SPLASHSCREEN_HPP
