#pragma once

class BoxCollideable;

class CameraManager : public sf::Drawable
{
public:
	CameraManager(sf::RenderWindow* window);

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	inline void SetFixedPoint(const sf::Vector2f& fixedPoint) 
	{
		m_FixedPoint = fixedPoint;
		m_CameraMode = CameraMode::CENTERING;
	};
	
	inline void SetBoxToFollow(const BoxCollideable* boxToFollow)
	{
		m_BoxToFollow = boxToFollow;
		m_CameraMode = CameraMode::FOLLOW;
	};

	//====================//
	
	bool DisplayCameraZones;

private:
	void MoveCameraView(const sf::Vector2f& offset);
	void FollowBox(float deltaTime, const sf::FloatRect& hardZone, const sf::FloatRect& softZone);
	void RecenterCamera(float deltaTime);

	//====================//

	enum class CameraMode // TODO : make it smaller since there are not a lot of options
	{
		FIXED		= 0,
		CENTERING	= 1,
		FOLLOW		= 2
	} m_CameraMode;

	sf::RenderWindow* m_Window;
	sf::View m_CameraView;

	sf::Vector2f m_FixedPoint;

	const BoxCollideable* m_BoxToFollow;
	sf::RectangleShape m_HardMoveZone;
	sf::RectangleShape m_SoftMoveZone;
	bool m_ExitedHardZone;
};