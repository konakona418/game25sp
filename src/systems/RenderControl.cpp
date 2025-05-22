// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#include "RenderControl.hpp"

#include "Common.hpp"
#include "components/Layout.hpp"
#include "components/Render.hpp"

void game::SRenderSystem::update(sf::RenderTarget& target, sf::Time deltaTime) {
    auto& registry = game::getRegistry();

    registry.sort<CRenderLayerComponent>(
        [](const auto& lhs, const auto& rhs) {
        return lhs.getLayer() < rhs.getLayer();
    });

    registry.sort<CRenderOrderComponent>(
        [](const auto& lhs, const auto& rhs) {
            return lhs.getOrder() < rhs.getOrder();
    });

    // make sure that the ordering is applied.
    // see also: https://github.com/skypjack/entt/issues/752
    // however I didn't notice any performance difference.

    auto spriteView = registry.view<CGlobalTransform, CRenderComponent, CRenderLayerComponent, CRenderOrderComponent, CSpriteRenderComponent>();

    for (auto entity : spriteView) {
        auto globalTransform = spriteView.get<CGlobalTransform>(entity);
        spriteView.get<CSpriteRenderComponent>(entity).update(target, globalTransform);
    }

    auto animatedSpriteView = registry.view<CGlobalTransform, CRenderComponent, CRenderLayerComponent, CRenderOrderComponent, CAnimatedSpriteRenderComponent>();
    for (auto entity : animatedSpriteView) {
        auto globalTransform = animatedSpriteView.get<CGlobalTransform>(entity);
        animatedSpriteView.get<CAnimatedSpriteRenderComponent>(entity).update(target, deltaTime, globalTransform);
    }
    // todo: implement other render systems
}
