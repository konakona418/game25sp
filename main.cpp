#include <iostream>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "Prelude.hpp"

int main(){
    game::Game& game = game::Game::createGame();
    game.getThreadPool().schedule([] -> void {
        std::cout << "Hello World!" << std::endl;
    });
    game.run();

}
