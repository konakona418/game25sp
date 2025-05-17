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

    auto spriteView = registry.view<CGlobalTransform, CRenderComponent, CRenderLayerComponent, CSpriteRenderComponent>();
    spriteView.use<CRenderLayerComponent>();
    // make sure that the ordering is applied.
    // see also: https://github.com/skypjack/entt/issues/752

    for (auto entity : spriteView) {
        auto globalTransform = spriteView.get<CGlobalTransform>(entity);
        spriteView.get<CSpriteRenderComponent>(entity).update(
            target, globalTransform.getPosition(),
            globalTransform.getSize(), globalTransform.getScale());
    }

    auto animatedSpriteView = registry.view<CGlobalTransform, CRenderComponent, CRenderLayerComponent, CAnimatedSpriteRenderComponent>();
    animatedSpriteView.use<CRenderLayerComponent>();
    for (auto entity : animatedSpriteView) {
        auto globalTransform = animatedSpriteView.get<CGlobalTransform>(entity);
        animatedSpriteView.get<CAnimatedSpriteRenderComponent>(entity).update(
            target, deltaTime, globalTransform.getPosition(),
            globalTransform.getSize(), globalTransform.getScale());
    }
    // todo: implement other render systems
}
