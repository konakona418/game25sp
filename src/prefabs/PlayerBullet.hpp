// Game - NWPU C++ sp25
// Created on 2025/5/26
// by konakona418 (https://github.com/konakona418)

#ifndef GAME25SP_PLAYERBULLET_HPP
#define GAME25SP_PLAYERBULLET_HPP

#include "SFML/System/Vector2.hpp"

#include "systems/SceneControl.hpp"
#include "components/Render.hpp"
#include "systems/CollisionControl.hpp"

namespace game::prefab {

    struct GPlayerBulletComponent {};

    class PlayerBullet : game::TreeLike {
    public:
        static PlayerBullet create() { return {}; }
        static PlayerBullet create(const sf::Vector2f& pos) { return PlayerBullet { pos }; }
    private:
        static constexpr float RENDER_LAYER = 25;
        static constexpr float SPEED = 600.f;

        PlayerBullet();
        explicit PlayerBullet(const sf::Vector2f& pos);

        static entt::resource<SpriteFrame> loadTexture();
        static void onUpdate(entt::entity entity, sf::Time deltaTime);
    };

} // game

#endif //GAME25SP_PLAYERBULLET_HPP
