// Game - NWPU C++ sp25
// Created on 2025/5/26
// by konakona418 (https://github.com/konakona418)

#include <cmath>

#include "PlayerBullet.hpp"
#include "utils/MovementUtils.hpp"
#include "components/Render.hpp"
#include "components/Velocity.hpp"
#include "components/Collision.hpp"
#include "components/Lighting.hpp"
#include "components/Scripts.hpp"
#include "utils/LazyLoader.hpp"
#include "ResourceManager.hpp"
#include "utils/TextureGenerator.hpp"

#include "Mob.hpp"

namespace game::prefab {
    PlayerBullet::PlayerBullet() : PlayerBullet({0.f, 0.f}) {
    }

    PlayerBullet::PlayerBullet(const sf::Vector2f& pos) : game::TreeLike() {
        static size_t renderOrderAccumulator = 0;

        auto& registry = game::getRegistry();
        auto entity = registry.create();
        m_entity = entity;

        game::MovementUtils::builder()
                .setLocalPosition(pos)
                .setSize({32, 32})
                .setScale({1.0, 1.0})
                .setAnchor(game::CLayout::Anchor::MiddleCenter())
                .build(entity);
        game::SceneTreeUtils::attachSceneTreeComponents(entity);

        registry.emplace<game::CRenderComponent>(entity);
        registry.emplace<game::CRenderLayerComponent>(entity, RENDER_LAYER, renderOrderAccumulator++);
        registry.emplace<game::CRenderTargetComponent>(entity, game::CRenderTargetComponent::GameComponent);

        auto frame = loadTexture();
        registry.emplace<game::CSpriteRenderComponent>(entity, frame);

        registry.emplace<game::CCollisionComponent>(entity);
        registry.emplace<game::CCollisionCircleComponent>(entity, 16.f);
        // on layer 3, collide with mobs(4)
        registry.emplace<game::CCollisionLayerComponent>(entity,
                                                         CollisionUtils::getCollisionMask(3), CollisionUtils::getCollisionMask(4));

        game::InvokeUpdateDelegate delegate;
        delegate.connect<&PlayerBullet::onUpdate>();
        registry.emplace<game::CScriptsComponent>(entity, delegate);

        registry.emplace<game::CLightingComponent>(entity, sf::Color(255, 192, 203, 240), 36.f);

        registry.emplace<game::prefab::GPlayerBulletComponent>(entity);
    }

    entt::resource<SpriteFrame> PlayerBullet::loadTexture() {
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

    void PlayerBullet::onUpdate(entt::entity entity, sf::Time deltaTime) {
        auto& registry = game::getRegistry();
        auto bulletPos = registry.get<game::CGlobalTransform>(entity).getPosition();

        entt::entity target { entt::null };
        float minDistance = std::numeric_limits<float>::max();
        sf::Vector2f targetPosition;

        registry.view<GMobComponent>().each([&](entt::entity entity, const GMobComponent&) {
            auto mobPosition = registry.get<game::CGlobalTransform>(entity).getPosition();
            auto distance = (mobPosition - bulletPos).lengthSquared();
            if (distance < minDistance) {
                minDistance = distance;
                target = entity;
                targetPosition = mobPosition;
            }
        });

        if (target != entt::null) {
            auto normal = (targetPosition - bulletPos).normalized();
            MovementUtils::move(entity, normal * SPEED * deltaTime.asSeconds());
        } else {
            UnmountUtils::queueUnmount(entity);
        }
    }
} // game