#pragma once

class Door : public sf::Drawable, public BoxCollideable
{
public:
	Door(const sf::Vector2f& centerPosition, const sf::Vector2f& size);
	~Door();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void RenderDebugMenu(sf::RenderTarget& target) 
	{
		ImGui::Checkbox("Open Door", &m_IsDoorOpen);
	};
	
	void OnTrigger(BoxCollideable* other) override;

	inline void OpenDoor() { m_IsDoorOpen = true; };
	inline const bool IsPlayingEndGame() const { return m_IsPlayingEndGame; };

private:
	void StartEndGame();

	//====================//

    sf::RectangleShape m_Rectangle;

	float m_rColor;
	float m_gColor;
	float m_bColor;

	bool m_IsDoorOpen;
	bool m_IsPlayingEndGame;
};