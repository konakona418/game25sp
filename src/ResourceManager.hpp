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

    struct RawTexture {
        sf::Texture texture;

        explicit RawTexture(const std::string& filename) : texture(filename) {};
    };

    struct RawTextureLoader {
        using result_type = std::shared_ptr<RawTexture>;

        result_type operator()(const std::string& name) const {
            return std::make_shared<RawTexture>(name);
        }
    };

    using RawTextureCache = entt::resource_cache<RawTexture, RawTextureLoader>;

    struct Texture {
        entt::resource<RawTexture> rawTextureRef;
        std::optional<sf::IntRect> textureRect { std::nullopt };

        explicit Texture(entt::resource<RawTexture> texture) : rawTextureRef(std::move(texture)) {};

        Texture(entt::resource<RawTexture> texture, const sf::IntRect& rect) : rawTextureRef(std::move(texture)), textureRect(rect) {};
    };

    struct TextureLoader {
        using result_type = std::shared_ptr<Texture>;

        result_type operator()(entt::resource<RawTexture> texture) const {
            return std::make_shared<Texture>(std::move(texture));
        }

        result_type operator()(entt::resource<RawTexture> texture, const sf::IntRect& rect) const {
            return std::make_shared<Texture>(std::move(texture), rect);
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

        static RawTextureCache& getRawTextureCache() {
            static RawTextureCache cache;
            return cache;
        }
    };

}



#endif //RESOURCE_HPP
