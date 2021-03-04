#pragma once

class SoulChunk : public sf::Drawable, public BoxCollideable 
{
public:
	SoulChunk(const sf::Texture& texture, const sf::Vector2f pos);
	~SoulChunk();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline const bool WasCollected() const { return m_WasCollected; };

private:
	sf::Sprite m_Sprite;
	sf::Vector2f m_Position;
	bool m_WasCollected;
};