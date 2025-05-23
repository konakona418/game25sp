// Game - NWPU C++ sp25
// Created on 2025/5/22
// by konakona418 (https://github.com/konakona418)

#include "Mob.hpp"

#include "Bullet.hpp"
#include "Player.hpp"
#include "components/Scripts.hpp"
#include "components/Velocity.hpp"
#include "utils/TextureGenerator.hpp"
#include "utils/LazyLoader.hpp"
#include "utils/MovementUtils.hpp"

namespace game::prefab {
    Mob Mob::create() {
        return create(sf::Vector2f {0.f, 0.f});
    }

    Mob Mob::create(sf::Vector2f pos) {
        return Mob { pos };
    }

    Mob::Mob(sf::Vector2f pos) {
        static size_t renderOrderAccumulator = 0;

        auto& registry = game::getRegistry();
        auto entity = registry.create();
        m_entity = entity;

        game::MovementUtils::builder()
            .setLocalPosition(pos)
            .setSize({32, 48})
            .setScale({1.0, 1.0})
            .setAnchor(game::CLayout::Anchor::MiddleCenter())
            .build(entity);
        game::SceneTreeUtils::attachSceneTreeComponents(entity);

        registry.emplace<game::CRenderComponent>(entity);
        registry.emplace<game::CRenderLayerComponent>(entity, RENDER_LAYER);
        registry.emplace<game::CRenderOrderComponent>(entity, renderOrderAccumulator++);

        auto animations = loadAnimationResources();
        registry.emplace<game::CAnimatedSpriteRenderComponent>(entity, (*animations)["idle"], true);
        InvokeUpdateDelegate delegate;
        delegate.connect<&Mob::mobUpdate>();

        registry.emplace<game::CScriptsComponent>(entity, delegate);
        registry.emplace<game::CVelocity>(entity);

        registry.emplace<game::prefab::GMobComponent>(entity, animations);
    }

    MobSharedAnimation Mob::loadAnimationResources() {
        static Lazy<MobSharedAnimation> animations {
            [] {
                auto res = std::make_shared<std::unordered_map<std::string, AnimatedFrames>>();
                res->emplace("idle", game::AnimatedTextureGenerator()
                                   .setOffset(sf::Vector2f{0, 0})
                                   .setPlacement(sf::Vector2u{1, 3})
                                   .setSize(sf::Vector2f{32, 48})
                                   .setDuration(sf::seconds(0.16))
                                   .generate("mobIdle", "slime.png"));
                return res;
            }
        };

        return *animations;
    }

    void Mob::mobUpdate(entt::entity entity, sf::Time deltaTime) {
        auto& registry = game::getRegistry();
        auto& mobComponent = registry.get<game::prefab::GMobComponent>(entity);

        auto& playerSelector = *registry.view<game::prefab::GPlayerComponent>().begin();
        if (!registry.valid(playerSelector)) {
            auto& velocityComponent = registry.get<game::CVelocity>(entity);
            velocityComponent.setAcceleration(sf::Vector2f {0, 0});
            velocityComponent.setVelocity(sf::Vector2f {0, 0});
            return;
        }

        const auto& playerPos = registry.get<game::CGlobalTransform>(playerSelector).getPosition();
        const auto& mobPos = registry.get<game::CGlobalTransform>(entity).getPosition();
        auto delta = (playerPos - mobPos).normalized();

        if (mobComponent.attackClock.getElapsedTime() > GMobComponent::ATTACK_INTERVAL && randomBool(0.75f)) {
            Bullet bullet = Bullet::create(mobPos, delta, random(100.f, 200.f));
            mobComponent.bullets.push_back(bullet.getEntity());

            if (mobComponent.bullets.size() > GMobComponent::MAX_BULLET_NUM) {
                SceneTreeUtils::unmount(mobComponent.bullets.front());
                mobComponent.bullets.pop_front();
            }

            mobComponent.attackClock.restart();
        }

        if (mobComponent.moveClock.getElapsedTime() > GMobComponent::MOVE_INTERVAL && randomBool(0.75f)) {
            auto& velocityComponent = registry.get<game::CVelocity>(entity);

            auto velocity = (delta + random({-0.1f, 0.1f}, {-0.1f, 0.1f})) * random(50.f, 100.f);
            auto acceleration = -velocity / (random(1.0f, 1.5f) * GMobComponent::MOVE_INTERVAL.asSeconds());

            velocityComponent.setAcceleration(acceleration);
            velocityComponent.setVelocity(velocity);

            // the asset is facing left, so we need to flip it back.
            // this behavior is quite absurd, so
            // use a better asset.
            if (velocity.x > 0) {
                mobComponent.flipH = true;
            } else {
                mobComponent.flipH = false;
            }

            mobComponent.moveClock.restart();
        }

        game::MovementUtils::flipHorizontal(entity, mobComponent.flipH);
    }
} // game