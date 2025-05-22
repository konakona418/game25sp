// Game - NWPU C++ sp25
// Created on 2025/5/22
// by konakona418 (https://github.com/konakona418)

#include "Bullet.hpp"

#include <utils/TextureGenerator.hpp>

#include "components/Render.hpp"
#include "components/Velocity.hpp"
#include "utils/LazyLoader.hpp"
#include "utils/MovementUtils.hpp"

namespace game::prefab {

    Bullet Bullet::create(sf::Vector2f pos, sf::Vector2f dir) {
        return { pos, dir };
    }

    Bullet Bullet::create(sf::Vector2f pos, sf::Vector2f dir, float speed) {
        return { pos, dir, speed };
    }

    void Bullet::onUpdate(entt::entity entity, sf::Time deltaTime) {}

    Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir) {
        create(pos, dir, DEFAULT_SPEED);
    }

    Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir, float speed) : TreeLike() {
        static size_t renderOrderAccumulator = 0;

        auto& registry = game::getRegistry();
        auto entity = registry.create();
        m_entity = entity;

        game::MovementUtils::builder()
            .setLocalPosition(pos)
            .setSize({32, 32})
            .setScale({0.25, 0.25})
            .setAnchor(game::CLayout::Anchor::MiddleCenter())
            .build(entity);
        game::SceneTreeUtils::attachSceneTreeComponents(entity);

        registry.emplace<game::CRenderComponent>(entity);
        registry.emplace<game::CRenderLayerComponent>(entity, RENDER_LAYER);
        registry.emplace<game::CRenderOrderComponent>(entity, renderOrderAccumulator++);

        auto frame = loadTexture();
        registry.emplace<game::CSpriteRenderComponent>(entity, frame);
        registry.emplace<game::CVelocity>(entity, dir.normalized() * speed);

        registry.emplace<game::prefab::GBulletComponent>(entity);
    }

    std::shared_ptr<SpriteFrame> Bullet::loadTexture() {
        static Lazy texture = Lazy<std::shared_ptr<SpriteFrame>> {
            []() {
                SpriteFrame frame = StaticTextureGenerator()
                                    .setOffset({0, 0})
                                    .setSize({32, 32})
                                    .generate("bullet", "bullet.png");
                std::shared_ptr<SpriteFrame> texture = std::make_shared<SpriteFrame>(frame);
                return texture;
            }
        };
        return *texture;
    }
} // game