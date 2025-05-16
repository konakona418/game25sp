// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#include "Render.hpp"

#include "Common.hpp"
#include "Logger.hpp"

void game::CSpriteRenderComponent::update(
    sf::RenderTarget& target, sf::Vector2f position, sf::Vector2f size, sf::Vector2f scale) {
    // this is not even a temporary solution.
    if (!m_sprite.has_value()) {
        m_sprite = sf::Sprite(m_frame.texture->texture);
    }
    m_sprite->setPosition(position);
    m_sprite->setScale(scale);

    if (m_frame.texture->textureRect.has_value()) {
        m_sprite->setTextureRect(m_frame.texture->textureRect.value());
    } else {
        m_sprite->setTextureRect({{0, 0}, {static_cast<int>(size.x), static_cast<int>(size.y)}});
    }
    target.draw(*m_sprite);
}
