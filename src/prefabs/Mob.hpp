// Game - NWPU C++ sp25
// Created on 2025/5/22
// by konakona418 (https://github.com/konakona418)

#ifndef MOB_HPP
#define MOB_HPP
#include <memory>
#include <unordered_map>

#include "components/Render.hpp"
#include "SFML/System/Clock.hpp"
#include "systems/SceneControl.hpp"

namespace game::prefab {

    using MobSharedAnimation = std::shared_ptr<std::unordered_map<std::string, entt::resource<AnimatedFrames>>>;

    struct GMobComponent {
        static constexpr sf::Time MOVE_INTERVAL = sf::seconds(1.5f);
        static constexpr sf::Time ATTACK_INTERVAL = sf::seconds(1.0f);
        static constexpr size_t MAX_BULLET_NUM = 4;

        bool flipH = false;
        MobSharedAnimation animations;
        float health = 100;

        sf::Clock moveClock;
        sf::Clock attackClock;

        GMobComponent() = delete;

        std::deque<entt::entity> bullets;

        explicit GMobComponent(MobSharedAnimation animations) : animations(std::move(animations)) {
            moveClock.restart();
            attackClock.restart();
        }
    };

    class Mob : public game::TreeLike {
    public:
        static Mob create();
        static Mob create(sf::Vector2f pos);
        ~Mob() = default;
    private:
        static constexpr size_t RENDER_LAYER = 12;

        explicit Mob(sf::Vector2f pos);
        static MobSharedAnimation loadAnimationResources();
        static void mobUpdate(entt::entity entity, sf::Time deltaTime);
    };

} // game

#endif //MOB_HPP
