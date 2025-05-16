// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#include "RenderControl.hpp"

#include "Common.hpp"
#include "components/Layout.hpp"
#include "components/Render.hpp"

void game::SRenderSystem::update(sf::RenderTarget& target) {
    auto& registry = game::getRegistry();
    auto spriteView = registry.view<CGlobalTransform, CRenderComponent, CSpriteRenderComponent>();
    for (auto entity : spriteView) {
        auto globalTransform = spriteView.get<CGlobalTransform>(entity);
        spriteView.get<CSpriteRenderComponent>(entity).update(
            target, globalTransform.getPosition(),
            globalTransform.getSize(), globalTransform.getScale());
    }
    // todo: implement other render systems
}
