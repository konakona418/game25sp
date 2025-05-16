// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <entt/resource/cache.hpp>

#include "SFML/Graphics/Texture.hpp"


namespace game {
    struct BinaryFile {
        std::string name {};
        std::vector<uint8_t> data;

        explicit BinaryFile(const std::string& name) {
            this->name = name;
            data.resize(std::filesystem::file_size(name));
            std::ifstream file(name, std::ios::binary);
            file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));
            file.close();
        }
    };

    struct BinaryFileLoader {
        using result_type = BinaryFile;
        result_type operator()(const std::string& name) const {
            return result_type { name };
        }
    };

    using BinaryFileCache = entt::resource_cache<BinaryFile, BinaryFileLoader>;

    struct Texture {
        sf::Texture texture;
        std::optional<sf::IntRect> textureRect { std::nullopt };

        explicit Texture(const std::string& filename) : texture(filename) {};
        Texture(const std::string& filename, const sf::IntRect& rect) : texture(filename), textureRect(rect) {};
    };

    struct TextureLoader {
        using result_type = std::shared_ptr<Texture>;
        result_type operator()(const std::string& name) const {
            return std::make_shared<Texture>(name);
        }

        result_type operator()(const std::string& name, const sf::IntRect& rect) const {
            return std::make_shared<Texture>(name, rect);
        }
    };

    using TextureCache = entt::resource_cache<Texture, TextureLoader>;

    struct ResourceManager {
        ResourceManager() = default;

        static BinaryFileCache& getBinaryFileCache() {
            static BinaryFileCache cache;
            return cache;
        }

        static TextureCache& getTextureCache() {
            static TextureCache cache;
            return cache;
        }
    };

}



#endif //RESOURCE_HPP
