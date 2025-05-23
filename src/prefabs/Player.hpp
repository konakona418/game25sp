// Game - NWPU C++ sp25
// Created on 2025/5/21
// by konakona418 (https://github.com/konakona418)

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include <unordered_map>

#include "components/Render.hpp"
#include "systems/SceneControl.hpp"


namespace game {
    struct EOnCollisionEvent;
}

namespace game::prefab {
    class Player;

    struct GPlayerComponent {
        GPlayerComponent() = default;
        bool flipH = false;
        std::unordered_map<std::string, entt::resource<AnimatedFrames>> animations;

        float health = 100.f;

        explicit GPlayerComponent(std::unordered_map<std::string, entt::resource<AnimatedFrames>> animations)
            : animations(std::move(animations)) {};
    };

    class Player : public game::TreeLike {
    public:
        static Player create();

    private:
        static constexpr size_t RENDER_LAYER = 16;

        Player();
        static std::unordered_map<std::string, entt::resource<game::AnimatedFrames>> loadAnimationResources();
        static void onUpdate(entt::entity entity, sf::Time deltaTime);
        static void onCollision(game::EOnCollisionEvent);
    };
}


#endif //PLAYER_HPP
