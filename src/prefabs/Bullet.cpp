// Game - NWPU C++ sp25
// Created on 2025/5/22
// by konakona418 (https://github.com/konakona418)

#include "Bullet.hpp"

#include <utils/TextureGenerator.hpp>

#include "ResourceManager.hpp"
#include "components/Collision.hpp"
#include "components/Render.hpp"
#include "components/Velocity.hpp"
#include "utils/LazyLoader.hpp"
#include "utils/MovementUtils.hpp"
#include "components/Lighting.hpp"

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
        registry.emplace<game::CRenderLayerComponent>(entity, RENDER_LAYER, renderOrderAccumulator++);
        registry.emplace<game::CRenderTargetComponent>(entity, game::CRenderTargetComponent::GameComponent);

        auto frame = loadTexture();
        registry.emplace<game::CSpriteRenderComponent>(entity, frame);
        registry.emplace<game::CVelocity>(entity, dir.normalized() * speed);

        registry.emplace<game::CCollisionComponent>(entity);
        registry.emplace<game::CCollisionCircleComponent>(entity, 16.f);
        // on layer 2, collide with player(1)
        registry.emplace<game::CCollisionLayerComponent>(entity,
            CollisionUtils::getCollisionMask(2), CollisionUtils::getCollisionMask(1));

        registry.emplace<game::CLightingComponent>(entity, sf::Color(255, 192, 203, 196), 12.f);

        registry.emplace<game::prefab::GBulletComponent>(entity);
    }

    entt::resource<SpriteFrame> Bullet::loadTexture() {
        static Lazy texture = Lazy<entt::resource<SpriteFrame>> {
            []() {
                entt::resource<SpriteFrame> texture =
                    ResourceManager::getSpriteFrameCache().load(
                        entt::hashed_string {  "bulletSprite" },
                        StaticTextureGenerator()
                                    .setOffset({0, 0})
                                    .setSize({32, 32})
                                    .generate("bullet", "assets/image/bullet.png"))
                    .first->second;
                return texture;
            }
        };
        return *texture;
    }
} // game