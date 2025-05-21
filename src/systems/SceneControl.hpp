// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#ifndef SCENECONTROL_HPP
#define SCENECONTROL_HPP

#include <entt/entt.hpp>


namespace game {
    struct CChild;
    struct CParent;
    struct CNode;

    class SceneTreeUtils {
    public:
        SceneTreeUtils() = default;

        /**
         * note that this function just initializes the entity with the scene tree components,
         * it does not add anything to the scene tree.
         * @param entity
         * @return
         */
        static entt::entity attachSceneTreeComponents(entt::entity entity);

        static entt::entity detachSceneTreeComponents(entt::entity entity);

        static entt::entity attachChild(entt::entity parent, entt::entity child);

        static entt::entity detachChild(entt::entity parent, entt::entity child);

        static entt::entity attachParent(entt::entity child, entt::entity parent);

        static entt::entity detachParent(entt::entity child, entt::entity parent);

        static entt::entity markAsDirty(entt::entity entity);

        static entt::entity markAsClean(entt::entity entity);

        static entt::entity markAsCleanRecurse(entt::entity entity);

        static bool isDirty(entt::entity entity);

        static void unmount(entt::entity entity);
    };

    class SScenePositionUpdateSystem {
    public:
        SScenePositionUpdateSystem() = default;

        static void update();

        /**
         * Every time the entity is updated, it should be manually marked as dirty.
         * @param entity
         */
        static void markEntityAsDirty(entt::entity entity);

        static void markEntityAsClean(entt::entity entity);
    private:
        static void calculateLayout(entt::entity entity);
    };

    class SSceneUnmountSystem {
    public:
        SSceneUnmountSystem() = default;

        static void update();

        static void unmount(entt::entity entity);
    };
}



#endif //SCENECONTROL_HPP
