// Game - NWPU C++ sp25
// Created on 2025/5/16
// by konakona418 (https://github.com/konakona418)

#ifndef RENDER_HPP
#define RENDER_HPP
#include <entt/resource/resource.hpp>
#include <utility>

#include "ResourceManager.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

namespace game {
    struct CRenderComponent {};

    // specify the layer for the component to be rendered
    struct CRenderLayerComponent {
        CRenderLayerComponent() = default;
        explicit CRenderLayerComponent(size_t layer) : m_layer(layer) {}
        [[nodiscard]] size_t getLayer() const { return m_layer; }
        void setLayer(size_t layer) { m_layer = layer; }
    private:
        size_t m_layer;
    };

    struct CRenderTargetComponent {};

    struct SpriteFrame {
        sf::Time duration;
        entt::resource<Texture> texture;

        SpriteFrame(entt::resource<Texture> texture, sf::Time duration) : duration(duration), texture(std::move(texture)) {}
        explicit SpriteFrame(entt::resource<Texture> texture) : duration(sf::seconds(1)), texture(std::move(texture)) {}
    };

    struct CSpriteRenderComponent {
        explicit CSpriteRenderComponent(SpriteFrame frame): m_frame(std::move(frame)) {}

        void setFrame(SpriteFrame frame) { m_frame = std::move(frame); }
        [[nodiscard]] SpriteFrame getFrame() const { return m_frame; }

        void update(sf::RenderTarget& target, sf::Vector2f position, sf::Vector2f size, sf::Vector2f scale, sf::Vector2f origin);
    private:
        SpriteFrame m_frame;
        std::optional<sf::Sprite> m_sprite;
    };

    struct CTextRenderComponent {};

    struct AnimatedFrames {
        std::vector<entt::resource<Texture>> frames;
        bool loop = true;
        sf::Time duration;
    };

    struct CAnimatedSpriteRenderComponent {
        explicit CAnimatedSpriteRenderComponent(AnimatedFrames frames, const bool loop = true)
            : m_frameControl(std::move(frames), loop) {}
        void setFrames(AnimatedFrames frames) { m_frameControl.m_frames = std::move(frames); }
        [[nodiscard]] AnimatedFrames getFrames() const { return m_frameControl.m_frames; }
        void update(sf::RenderTarget& target, sf::Time deltaTime,
                    sf::Vector2f position, sf::Vector2f size, sf::Vector2f scale, sf::Vector2f origin);

    private:
        struct FrameControl {
            AnimatedFrames m_frames;
            size_t m_frameIndex;
            size_t m_frameCount;
            bool m_loop;
            sf::Time m_timeAccumulated = sf::Time::Zero;

            FrameControl(AnimatedFrames frames, bool loop)
                : m_frames(std::move(frames)), m_frameIndex(0), m_frameCount(m_frames.frames.size()), m_loop(loop) {}
            void update(sf::Time deltaTime);
            void nextFrame();
            [[nodiscard]] entt::resource<Texture> getCurrentFrame() const;
            void reset();
        };

        FrameControl m_frameControl;
        std::optional<sf::Sprite> m_sprite;
    };

    using TileIdType = uint32_t;
    struct Tile {
        AnimatedFrames frames {};
        TileIdType id {};
    };

    struct SingleTileItem {
        TileIdType tileId;
        sf::Vector2i tilePlacement;
        sf::Vector2i tileSize;
        std::optional<sf::Sprite> sprite;

        SingleTileItem(TileIdType tileId, sf::Vector2i tilePlacement, sf::Vector2i tileSize)
            : tileId(tileId), tilePlacement(tilePlacement), tileSize(tileSize) {}
    };

    struct CTiledRenderComponent {
        explicit CTiledRenderComponent(sf::Vector2f baseTilePixelSize) : m_tileControl(baseTilePixelSize) {}
        void addTile(TileIdType id, AnimatedFrames frames);
        void addTile(Tile tile);

        void addTile(TileIdType id, sf::Vector2i tilePlacement, sf::Vector2i tileSize);
        void addTile(const SingleTileItem& tileItem);

        void update(sf::RenderTarget& target, sf::Time deltaTime,
            sf::Vector2f position, sf::Vector2f size, sf::Vector2f scale);
    private:
        struct TileControl {
            std::unordered_map<TileIdType, Tile> m_tiles {};
            std::vector<SingleTileItem> m_tileItemList {};
            sf::Vector2f m_baseTilePixelSize;

            void update(sf::Time deltaTime);
            void reset();
            Tile& getTileById(TileIdType id);

            explicit TileControl(sf::Vector2f baseTilePixelSize) : m_baseTilePixelSize(baseTilePixelSize) {};
        };
        TileControl m_tileControl;

    };
} // game

#endif //RENDER_HPP
