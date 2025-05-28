// Game - NWPU C++ sp25
// Created on 2025/5/28
// by konakona418 (https://github.com/konakona418)

#include "SplashScreen.hpp"
#include "utils/LazyLoader.hpp"
#include "ResourceManager.hpp"
#include "utils/TextureGenerator.hpp"
#include "utils/MovementUtils.hpp"
#include "components/Tweening.hpp"

namespace game::prefab {
    SplashScreen::SplashScreen() : game::TreeLike() {
        auto& registry = game::getRegistry();
        auto entity = registry.create();
        m_entity = entity;

        game::MovementUtils::builder()
                .setLocalPosition({0.f, 0.f})
                .setSize({1280.f, 720.f})
                .setScale({1.0, 1.0})
                .setAnchor(game::CLayout::Anchor::TopLeft())
                .build(entity);
        game::SceneTreeUtils::attachSceneTreeComponents(entity);

        registry.emplace<game::CRenderComponent>(entity);
        registry.emplace<game::CRenderLayerComponent>(entity, 0, 0);
        registry.emplace<game::CRenderTargetComponent>(entity, game::CRenderTargetComponent::UI);

        auto frame = loadImage();
        registry.emplace<game::CSpriteRenderComponent>(entity, frame);

        auto& tweenComponent = registry.emplace<game::CTweenComponent>(entity);
        tweenComponent.setCompletionCallback(&SplashScreen::onTweenCompleted);
        tweenComponent.setBeginValue(0.f);
        tweenComponent.setEndValue(1.f);
        tweenComponent.setDuration(sf::seconds(SPLASH_SCREEN_DURATION));
        tweenComponent.restart();
    }

    entt::resource<game::SpriteFrame> SplashScreen::loadImage() {
        static Lazy image = Lazy<entt::resource<game::SpriteFrame>> {
            [] {
                return ResourceManager::getSpriteFrameCache()
                    .load(entt::hashed_string { "splashScreenSprite" },
                          StaticTextureGenerator()
                            .setOffset({0, 0})
                            .setSize({32, 32})
                            .generate("splashScreen",
                                      "assets/image/splash-screen.png"))
                                      .first->second;
            }
        };
        return *image;
    }

    void SplashScreen::onTweenCompleted(entt::entity entity) {
        game::getEventDispatcher().trigger<EOnSplashScreenCompletedEvent>(EOnSplashScreenCompletedEvent { entity });
    }
} // game