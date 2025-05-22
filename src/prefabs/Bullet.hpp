// Game - NWPU C++ sp25
// Created on 2025/5/22
// by konakona418 (https://github.com/konakona418)

#ifndef BULLET_HPP
#define BULLET_HPP

#include "components/Render.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include "systems/SceneControl.hpp"

namespace game::prefab {

    struct GBulletComponent {
        GBulletComponent() = default;
    };

    class Bullet : public game::TreeLike {
    public:
        static Bullet create(sf::Vector2f pos, sf::Vector2f dir);
        static Bullet create(sf::Vector2f pos, sf::Vector2f dir, float speed);

        ~Bullet() = default;
        Bullet() = delete;

    private:
        static constexpr float DEFAULT_SPEED = 256.f;
        static constexpr size_t RENDER_LAYER = 24;

        void onUpdate(entt::entity entity, sf::Time deltaTime);
        Bullet(sf::Vector2f pos, sf::Vector2f dir);
        Bullet(sf::Vector2f pos, sf::Vector2f dir, float speed);

        static std::shared_ptr<SpriteFrame> loadTexture();
    };

} // game

#endif //BULLET_HPP
