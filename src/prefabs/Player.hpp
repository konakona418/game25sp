// Game - NWPU C++ sp25
// Created on 2025/5/21
// by konakona418 (https://github.com/konakona418)

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include <unordered_map>

#include "components/Render.hpp"


namespace game::prefab {
    class Player;

    struct GPlayerComponent {
        GPlayerComponent() = default;
        bool flipH = false;
        std::unordered_map<std::string, AnimatedFrames> animations;

        explicit GPlayerComponent(std::unordered_map<std::string, AnimatedFrames> animations)
            : animations(std::move(animations)) {};
    };

    class Player {
    public:
        static Player create();
        void detach() const;

    private:
        static constexpr size_t RENDER_LAYER = 16;
        entt::entity m_entity;

        Player();
        static std::unordered_map<std::string, AnimatedFrames> loadAnimationResources();
    };
}


#endif //PLAYER_HPP
