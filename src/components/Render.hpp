// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#ifndef RENDER_HPP
#define RENDER_HPP
#include <entt/resource/resource.hpp>
#include <utility>

#include "ResourceManager.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

namespace game {
    struct CRenderComponent {};

    struct SpriteFrame {
        sf::Time duration;
        entt::resource<Texture> texture;

        SpriteFrame(entt::resource<Texture> texture, sf::Time duration) : duration(duration), texture(std::move(texture)) {}
        explicit SpriteFrame(entt::resource<Texture> texture) : duration(sf::seconds(1)), texture(std::move(texture)) {}
    };

    struct CSpriteRenderComponent {
        explicit CSpriteRenderComponent(SpriteFrame frame): m_frame(std::move(frame)) {}

        void setFrame(SpriteFrame frame) { m_frame = std::move(frame); }
        [[nodiscard]] SpriteFrame getFrame() const { return m_frame; }

        void update(sf::RenderTarget& target, sf::Vector2f position, sf::Vector2f size, sf::Vector2f scale);
    private:
        SpriteFrame m_frame;
        std::optional<sf::Sprite> m_sprite;
    };

    struct CTextRenderComponent {};

    struct AnimatedFrames {
        std::vector<entt::resource<Texture>> frames;
        bool loop = true;
        sf::Time duration;
        size_t currentFrame = 0;
    };

    struct CAnimatedSpriteRenderComponent {};
    struct CTiledRenderComponent {};
} // game

#endif //RENDER_HPP
