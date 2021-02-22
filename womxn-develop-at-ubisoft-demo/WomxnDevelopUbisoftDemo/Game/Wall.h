#pragma once

class Wall : public sf::Drawable, public BoxCollideable
{
public:
	Wall(float xCenterPos, float yCenterPos, float width, float height);
	~Wall();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
private:
	sf::RectangleShape m_Rectangle;
};

