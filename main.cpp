#include "Prelude.hpp"
#include "prefabs/Bullet.hpp"
#include "prefabs/DialogBox.hpp"
#include "prefabs/Mob.hpp"
#include "prefabs/Player.hpp"
#include "prefabs/Root.hpp"
#include "systems/MusicControl.hpp"

int main() {
    game::Game& game = game::Game::createGame();
    game.setConfig({.windowTitle = "Game - C++ 25sp", .fps = 60, .vsync = false});

    // auto music = game::ResourceManager::getBinaryFileCache().load(entt::hashed_string {"2da_bloom_99.ogg"}, "2da_bloom_99.ogg").first->second;
    // game::SMusicSystem::playMusic(music, game::MusicConfig { true });

    game::prefab::Root root = game::prefab::Root::create();
    game::prefab::Player player = game::prefab::Player::create();
    game::prefab::DialogBox dialogBox = game::prefab::DialogBox::create();
    dialogBox.setVisibility(true);

    root.mountChild(dialogBox.getEntity());
    root.mountChild(player.getEntity());

    for (int i = 0; i < 16; i++) {
        game::prefab::Mob mob = game::prefab::Mob::create(game::random({ 1000.f, 500.f }));
        root.mountChild(mob.getEntity());
    }

    game.run();
    root.unmount();

    assert(player.isUnmounted());
    assert(root.isUnmounted());
}
