#include "Prelude.hpp"
#include "prefabs/Bullet.hpp"
#include "prefabs/DialogBox.hpp"
#include "prefabs/Mob.hpp"
#include "prefabs/Player.hpp"
#include "prefabs/Root.hpp"
#include "systems/MusicControl.hpp"
#include "utils/DialogGenerator.hpp"

void onPlayerDeath(game::prefab::EOnPlayerDeathEvent e) {
    auto& root = game::getRegistry().ctx().get<game::prefab::Root>();
    root.unmount();
}

void onDialogCompleted(game::prefab::EOnDialogBoxCompletedEvent e) {
    game::prefab::Player player = game::prefab::Player::create();
    auto& root = game::getRegistry().ctx().get<game::prefab::Root>();

    root.mountChild(player.getEntity());
    game::SceneTreeUtils::unmount(e.entity);

    for (int i = 0; i < 16; i++) {
        game::prefab::Mob mob = game::prefab::Mob::create(game::random({ 1000.f, 500.f }));
        root.mountChild(mob.getEntity());
    }
    game::getEventDispatcher().sink<game::prefab::EOnPlayerDeathEvent>().connect<&onPlayerDeath>();
}

int main() {
    game::Game& game = game::Game::createGame();
    game.setConfig({.windowTitle = "Game - C++ 25sp", .fps = 60, .vsync = false});

    game::prefab::Root root = game::prefab::Root::create();
    game::getRegistry().ctx().emplace<game::prefab::Root>(root);

    // auto music = game::ResourceManager::getBinaryFileCache().load(entt::hashed_string {"2da_bloom_99.ogg"}, "2da_bloom_99.ogg").first->second;
    // game::SMusicSystem::playMusic(music, game::MusicConfig { true });


    game::prefab::DialogBox dialogBox = game::prefab::DialogBox::create();

    auto texture0 = game::ResourceManager::getTextureCache()
            .load(entt::hashed_string { "portrait0" }, "assets/character0.png", sf::IntRect {{0, 0}, {1024, 1536}}).first->second;

    auto texture1 = game::ResourceManager::getTextureCache()
            .load(entt::hashed_string { "portrait1" }, "assets/character1.png", sf::IntRect {{0, 0}, {1024, 1536}}).first->second;
    auto dialogs = game::DialogGenerator()
            .fromJson("assets/dialog/start.json")
            .generate();
    dialogBox.loadDialog("dialog", dialogs);
    dialogBox.setVisibility(true);
    game::getEventDispatcher().sink<game::prefab::EOnDialogBoxCompletedEvent>().connect<&onDialogCompleted>();

    game.run();
}
