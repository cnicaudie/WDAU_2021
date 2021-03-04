#pragma once

#include "Tile.h"
#include <Game/Objects/SoulChunk.h>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void Update(float deltaTime);
    bool Load(const sf::Texture& tileset, const std::vector<int>& tiles, const sf::Vector2u& levelSize);

    inline const std::vector<std::shared_ptr<Tile>>& GetTileMap() const { return m_TileMap; };
    inline const std::vector<SoulChunk>& GetSoulChunks() const { return m_SoulChunks; }

private:
    sf::VertexArray m_Vertices;
    sf::Texture m_Tileset;

    std::vector<std::shared_ptr<Tile>> m_TileMap;

    // TODO : is it the right position ?
    std::vector<SoulChunk> m_SoulChunks;
};