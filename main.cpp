#include <SFML/Graphics.hpp>

#include "Prelude.hpp"
#include "prefabs/Player.hpp"
#include "prefabs/Root.hpp"

int main() {
    game::Game& game = game::Game::createGame();
    game.setConfig({.windowTitle = "Game - C++ 25sp", .fps = 60, .vsync = false});

    game::prefab::Root root = game::prefab::Root::create();
    game::prefab::Player player = game::prefab::Player::create();
    root.mountChild(player.getEntity());

    game.run();
    root.detach();

}
