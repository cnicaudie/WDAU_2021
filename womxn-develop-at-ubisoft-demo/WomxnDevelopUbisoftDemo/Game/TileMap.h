#pragma once

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool Load(const sf::Texture& tileset, const std::vector<int>& tiles, const sf::Vector2u& levelSize);

private:
    // TODO : Add a Tile class (with coordinates -> derived class for collideables...etc)

    sf::VertexArray m_Vertices;
    sf::Texture m_Tileset;
};