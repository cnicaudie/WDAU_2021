#pragma once

#include "Wall.h"
#include <vector>

class MainCharacter : public sf::Drawable, public BoxCollideable
{
public:	
	MainCharacter();

	void Update(float deltaTime);
	void ComputeVelocity();
	void Move(float deltaTime);
	bool CheckCollision();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void StartEndGame();

	inline void InitColliders(const std::vector<Wall>& walls) { m_Walls = walls; };

private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	bool m_IsUsingJoystick;
	unsigned int m_JoystickIndex;
	bool m_WasButtonPressed;

	sf::Vector2f m_Position; 
	sf::Vector2f m_Velocity;
	bool m_IsPlayingEndGame;

	std::vector<Wall> m_Walls;
};