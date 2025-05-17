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
        if (m_frame.texture->textureRect.has_value()) {
            m_sprite = sf::Sprite(m_frame.texture->texture, m_frame.texture->textureRect.value());
        } else {
            m_sprite = sf::Sprite(m_frame.texture->texture);
        }
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

void game::CAnimatedSpriteRenderComponent::update(sf::RenderTarget& target, sf::Time deltaTime, sf::Vector2f position,
    sf::Vector2f size, sf::Vector2f scale) {
    if (!m_sprite.has_value()) {
        if (m_frameControl.getCurrentFrame()->textureRect.has_value()) {
            m_sprite = sf::Sprite(m_frameControl.getCurrentFrame()->texture,
                m_frameControl.getCurrentFrame()->textureRect.value());
        } else {
            m_sprite = sf::Sprite(m_frameControl.getCurrentFrame()->texture);
        }
    }

    m_sprite->setPosition(position);
    m_sprite->setScale(scale);

    const auto currentFrame = m_frameControl.getCurrentFrame();
    m_sprite->setTexture(currentFrame->texture);
    if (m_frameControl.getCurrentFrame()->textureRect.has_value()) {
        m_sprite->setTextureRect(currentFrame->textureRect.value());
    } else {
        m_sprite->setTextureRect({{0, 0}, {static_cast<int>(size.x), static_cast<int>(size.y)}});
    }
    target.draw(*m_sprite);
}

void game::CAnimatedSpriteRenderComponent::FrameControl::update(const sf::Time deltaTime) {
    const float frameOffset = deltaTime / m_frames.duration;
    for (size_t i = 0; i < static_cast<size_t>(std::floor(frameOffset)); i++) {
        nextFrame();
    }
}

void game::CAnimatedSpriteRenderComponent::FrameControl::nextFrame() {
    m_frameIndex = (m_frameIndex + 1) % m_frameCount;
}

entt::resource<game::Texture> game::CAnimatedSpriteRenderComponent::FrameControl::getCurrentFrame() const {
    return m_frames.frames[m_frameIndex];
}
void game::CAnimatedSpriteRenderComponent::FrameControl::reset() {
    m_frameIndex = 0;
}
