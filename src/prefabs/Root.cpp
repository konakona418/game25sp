// Game - NWPU C++ sp25
// Created on 2025/5/21
// by konakona418 (https://github.com/konakona418)

#include "Root.hpp"

#include "Common.hpp"
#include "systems/SceneControl.hpp"
#include "utils/MovementUtils.hpp"

game::prefab::Root& game::prefab::Root::create() {
    auto& registry = getRegistry();
    static Root instance {};
    instance.m_entity = registry.create();

    MovementUtils::attachLayoutComponents(instance.m_entity);
    SceneTreeUtils::attachSceneTreeComponents(instance.m_entity);

    return instance;
}

void game::prefab::Root::detach() const {
    static bool isDetached = false;
    if (isDetached) {
        return;
    }
    SceneTreeUtils::unmount(m_entity);
    isDetached = true;
}

void game::prefab::Root::mountChild(entt::entity child) const {
    SceneTreeUtils::attachChild(m_entity, child);
}

game::prefab::Root::~Root() {
    detach();
}
