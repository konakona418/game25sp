#include <iostream>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "Prelude.hpp"

int main(){
    game::Game& game = game::Game::createGame();
    std::vector<game::Task> tasks;
    for (int i = 0; i < 10; i++) {
        tasks.emplace_back([i]() {
            std::this_thread::sleep_for(std::chrono::seconds(i));
            std::stringstream ss;
            ss << "Hello from thread ";
            ss << std::this_thread::get_id();
            game::getLogger().logInfo(ss.str());
        });
    }
    game::getThreadPool().waitForAll(tasks);
    game.run();

}
