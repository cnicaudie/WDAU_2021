#pragma once

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool LoadTileset(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

private:
    // TODO : Add function to parse a text file containing the level (FileManager ?)
    // TODO : Add a LevelManager (with LoadLevel function)
    // TODO : Add a Tile class (with coordinates -> derived class for collideables...etc)

    sf::VertexArray m_Vertices;
    sf::Texture m_Tileset; // TODO : Move in TextureManager
};