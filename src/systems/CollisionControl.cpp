// Game - NWPU C++ sp25
// Created on 2025/5/20
// by konakona418 (https://github.com/konakona418)

#include "CollisionControl.hpp"

#include "Common.hpp"
#include "Game.hpp"
#include "components/Collision.hpp"
#include "components/Layout.hpp"

namespace game {
    struct CollisionGrid {
        ssize_t x;
        ssize_t y;

        bool operator==(const CollisionGrid& other) const {
            return x == other.x && y == other.y;
        }
    };
}

namespace std {
    template <>
    struct hash<std::pair<ssize_t, ssize_t>> {
        size_t operator()(const std::pair<ssize_t, ssize_t>& p) const {
            constexpr size_t MAGIC = 0x9e3779b9;
            auto h1 = std::hash<ssize_t> {}(p.first);
            auto h2 = std::hash<ssize_t> {}(p.second);
            return h1 + MAGIC + (h1 << 6) + (h1 >> 2) ^ h2;
        }
    };
}

namespace game {
    void SCollisionSystem::update(sf::Time deltaTime) {
        using CollisionInfoTuple = std::tuple<std::vector<entt::entity>, size_t, size_t>;

        auto& registry = getRegistry();
        auto view = registry.view<CCollisionComponent, CCollisionLayerComponent>();

#ifdef GAME_USE_LEGACY_COLLISION

        for (auto it1 = view.begin(); it1 != view.end(); ++it1) {
                for (auto it2 = std::next(it1); it2 != view.end(); ++it2) {
                    if (!registry.valid(*it1) || !registry.valid(*it2)) {
                        getLogger().logWarn("CollisionSystem: Invalid entity");
                        continue;
                    }

                    auto& layer1 = view.get<CCollisionLayerComponent>(*it1);
                    auto& layer2 = view.get<CCollisionLayerComponent>(*it2);

                    if (!CollisionUtils::shouldCollide(
                            layer1.getLayer(), layer2.getLayer(),
                            layer1.getMask(), layer2.getMask())) {
                        continue;
                    }

                    bool collision = false;
                    collision |= checkCollisionBoxes(registry, *it1, *it2);
                    collision |= checkCollisionCircles(registry, *it1, *it2);
                    collision |= checkCollisionBoxCircle(registry, *it1, *it2);

                    if (collision) {
                        emitSignal(registry, *it1, *it2);
                    }
                }
            }
#else
        constexpr sf::Vector2f GRID_SIZE = { 48.f, 48.f };

        std::unordered_map<std::pair<ssize_t, ssize_t>, CollisionInfoTuple> collisionGrid;
        for (auto entity : view) {
            // todo: rounding issue
            // this mapping uses std::floor,
            // which may lead to unexpected behavior when the entity is placed on the edge of the grid
            auto grid = mapGrid(registry.get<CGlobalTransform>(entity).getPosition(), GRID_SIZE);
            auto& singleGrid = collisionGrid[grid];
            std::get<0>(singleGrid).push_back(entity);

            auto& collisionLayer = view.get<CCollisionLayerComponent>(entity);
            std::get<1>(singleGrid) |= collisionLayer.getLayer();
            std::get<2>(singleGrid) |= collisionLayer.getMask();
        }
        //getLogger().logDebug("CollisionSystem: Collision grid size: " + std::to_string(collisionGrid.size()));

        for (auto& [grid, tuple] : collisionGrid) {
            if (!(std::get<1>(tuple) & std::get<2>(tuple))) {
                continue;
            }

            auto& entities = std::get<0>(tuple);
            for (auto it1 = entities.begin(); it1 != entities.end(); ++it1) {
                for (auto it2 = std::next(it1); it2 != entities.end(); ++it2) {
                    if (!registry.valid(*it1) || !registry.valid(*it2)) {
                        getLogger().logWarn("CollisionSystem: Invalid entity");
                        continue;
                    }

                    auto& layer1 = view.get<CCollisionLayerComponent>(*it1);
                    auto& layer2 = view.get<CCollisionLayerComponent>(*it2);

                    if (!CollisionUtils::shouldCollide(
                            layer1.getLayer(), layer2.getLayer(),
                            layer1.getMask(), layer2.getMask())) {
                        continue;
                    }

                    bool collision = false;
                    collision |= checkCollisionBoxes(registry, *it1, *it2);
                    collision |= checkCollisionCircles(registry, *it1, *it2);
                    collision |= checkCollisionBoxCircle(registry, *it1, *it2);

                    if (collision) {
                        emitSignal(registry, *it1, *it2);
                    }
                }
            }
        }
#endif

    }

    bool SCollisionSystem::checkCollisionBoxes(entt::registry& reg, const entt::entity& entity1, const entt::entity& entity2) {
        if (!(reg.any_of<CCollisionAABBComponent>(entity1) && reg.any_of<CCollisionAABBComponent>(entity2))) {
            return false;
        }
        const auto& pos1 = reg.get<CGlobalTransform>(entity1).getPosition();
        const auto& pos2 = reg.get<CGlobalTransform>(entity2).getPosition();

        const auto& boxSize1 = reg.get<CCollisionAABBComponent>(entity1).getBoundingBox();
        const auto& boxSize2 = reg.get<CCollisionAABBComponent>(entity2).getBoundingBox();

        return sf::FloatRect { pos1, boxSize1 }
            .findIntersection(sf::FloatRect { pos2, boxSize2 })
            .has_value();
    }

    bool SCollisionSystem::checkCollisionCircles(entt::registry& reg, const entt::entity& entity1, const entt::entity& entity2) {
        if (!(reg.any_of<CCollisionCircleComponent>(entity1) && reg.any_of<CCollisionCircleComponent>(entity2))) {
            return false;
        }
        const auto& pos1 = reg.get<CGlobalTransform>(entity1).getPosition();
        const auto& pos2 = reg.get<CGlobalTransform>(entity2).getPosition();

        auto rad1 = reg.get<CCollisionCircleComponent>(entity1).getRadius();
        auto rad2 = reg.get<CCollisionCircleComponent>(entity2).getRadius();

        return (pos1 - pos2).lengthSquared() < std::powf(rad1 + rad2, 2);
    }

    bool SCollisionSystem::checkCollisionBoxCircle(entt::registry& reg, const entt::entity& entity1, const entt::entity& entity2) {
        if (!(reg.any_of<CCollisionAABBComponent>(entity1) && reg.any_of<CCollisionCircleComponent>(entity2)) &&
            !(reg.any_of<CCollisionCircleComponent>(entity1) && reg.any_of<CCollisionAABBComponent>(entity2))) {
            return false;
        }
        if (reg.any_of<CCollisionAABBComponent>(entity1)) {
            const auto& pos1 = reg.get<CGlobalTransform>(entity1).getPosition();
            const auto& boxSize1 = reg.get<CCollisionAABBComponent>(entity1).getBoundingBox();
            auto rad1 = boxSize1.length() / 2;

            const auto& pos2 = reg.get<CGlobalTransform>(entity2).getPosition();
            auto rad2 = reg.get<CCollisionCircleComponent>(entity2).getRadius();
            return (pos1 - pos2).lengthSquared() < std::powf(rad1 + rad2, 2);
        }
        if (reg.any_of<CCollisionCircleComponent>(entity1)) {
            const auto& pos1 = reg.get<CGlobalTransform>(entity1).getPosition();
            auto rad1 = reg.get<CCollisionCircleComponent>(entity1).getRadius();

            const auto& pos2 = reg.get<CGlobalTransform>(entity2).getPosition();
            const auto& boxSize2 = reg.get<CCollisionAABBComponent>(entity2).getBoundingBox();
            auto rad2 = boxSize2.length() / 2;
            return (pos1 - pos2).lengthSquared() < std::powf(rad1 + rad2, 2);
        }
        return false;
    }

    void SCollisionSystem::emitSignal(entt::registry& reg, const entt::entity& entity1, const entt::entity& entity2) {
        getGame().getEventDispatcher().trigger<EOnCollisionEvent>(EOnCollisionEvent { entity1, entity2 });
    }

    std::pair<ssize_t, ssize_t> SCollisionSystem::mapGrid(const sf::Vector2f& position, sf::Vector2f gridSize) {
        return {
            static_cast<ssize_t>(std::floor(position.x / gridSize.x)),
            static_cast<ssize_t>(std::floor(position.y / gridSize.y))
        };
    }
} // game