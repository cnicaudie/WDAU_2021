#pragma once

class CameraManager : public sf::Drawable
{
public:
	CameraManager(sf::RenderWindow* window, const Player* player);

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool DisplayCameraZones;

private:
	sf::RenderWindow* m_Window;
	sf::View m_CameraView;

	sf::RectangleShape m_HardMoveZone;
	sf::RectangleShape m_SoftMoveZone;

	const Player* m_Player;
};