#include <iostream>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "Prelude.hpp"
#include "components/Render.hpp"
#include "components/Scripts.hpp"
#include "prefabs/Player.hpp"
#include "systems/MovementControl.hpp"

#include "test/SceneTreeTests.hpp"
#include "utils/AnimatedTextureGenerator.hpp"

int main() {
    game::Game& game = game::Game::createGame();
    //testSceneTree();
    game.setConfig({.windowTitle = "Game - C++ 25sp", .fps = 60, .vsync = false});

    game::prefab::Player player = game::prefab::Player::create();

    game.run();
    player.detach();

}
