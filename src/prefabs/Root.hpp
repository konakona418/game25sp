// Game - NWPU C++ sp25
// Created on 2025/5/21
// by konakona418 (https://github.com/konakona418)

#ifndef ROOT_HPP
#define ROOT_HPP
#include <entt/entity/entity.hpp>

namespace game::prefab {
    class Root {
    public:
        static Root& create();
        void detach() const;
        void mountChild(entt::entity child) const;

        ~Root();
    private:
        entt::entity m_entity;
        Root() = default;
    };
} // game

#endif //ROOT_HPP
