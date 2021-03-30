#include <stdafx.h>
#include "CameraManager.h"

CameraManager::CameraManager(sf::RenderWindow* window)
    : m_Window(window)
    , m_CameraView(sf::FloatRect(0.f, 0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)))
    , m_ExitedHardZone(false)
    , DisplayCameraZones(false)
{
    const sf::Vector2f WINDOW_CENTER{ m_CameraView.getCenter() };
    const sf::Vector2f HARD_ZONE_SIZE{ m_Window->getSize().x / 1.6f, m_Window->getSize().y / 2.4f };
    const sf::Vector2f SOFT_ZONE_SIZE{ m_Window->getSize().x / 3.2f, m_Window->getSize().y / 4.8f };

    m_HardMoveZone.setPosition(WINDOW_CENTER);
    m_SoftMoveZone.setPosition(WINDOW_CENTER);

    m_HardMoveZone.setSize(HARD_ZONE_SIZE);
    m_SoftMoveZone.setSize(SOFT_ZONE_SIZE);

    m_HardMoveZone.setFillColor(sf::Color::Transparent);
    m_SoftMoveZone.setFillColor(sf::Color::Transparent);

    m_HardMoveZone.setOutlineColor(sf::Color::Red);
    m_SoftMoveZone.setOutlineColor(sf::Color::Magenta);

    m_HardMoveZone.setOutlineThickness(1);
    m_SoftMoveZone.setOutlineThickness(1);

    m_Window->setView(m_CameraView);
}

void CameraManager::Update(float deltaTime) 
{
    sf::FloatRect hardMoveZoneBoundingBox = m_HardMoveZone.getGlobalBounds();
    sf::FloatRect softMoveZoneBoundingBox = m_SoftMoveZone.getGlobalBounds();
    
    if (m_CameraMode == CameraMode::FOLLOW)
    {
        FollowBox(deltaTime, hardMoveZoneBoundingBox, softMoveZoneBoundingBox);
    } 
    else 
    {
        if (!softMoveZoneBoundingBox.contains(m_BoxToFollow->GetCenter()))
        {
            m_CameraMode = CameraMode::FOLLOW;
            FollowBox(deltaTime, hardMoveZoneBoundingBox, softMoveZoneBoundingBox);
        }
        else if (m_CameraMode == CameraMode::CENTERING) 
        {
            RecenterCamera(deltaTime);
        }
    }

    m_Window->setView(m_CameraView);
}

void CameraManager::FollowBox(float deltaTime, const sf::FloatRect& hardZone, const sf::FloatRect& softZone)
{
    const sf::Vector2f boxToFollowPosition = m_BoxToFollow->GetCenter();

    float followSpeedRate = 1.f;

    if (!hardZone.contains(boxToFollowPosition))
    {
        m_ExitedHardZone = true;
    } 
    else if (m_ExitedHardZone)
    {
        if (softZone.contains(boxToFollowPosition))
        {
            m_ExitedHardZone = false;
            followSpeedRate = 0.5f;
        }
    } 
    else if (!softZone.contains(boxToFollowPosition))
    {
        followSpeedRate = 0.5f;
    }
    else 
    {
        //followSpeedRate = 0.4f;
        SetFixedPoint(boxToFollowPosition);
    }

    MoveCameraView((boxToFollowPosition - m_CameraView.getCenter()) * followSpeedRate * deltaTime);
}

void CameraManager::RecenterCamera(float deltaTime)
{
    const float RECENTER_SPEED_RATE = 0.65f;
    const double RECENTER_DISTANCE_THRESHOLD = 30.;

    // TODO : Make a getDistance(vector, vector) function in a MathUtils file
    sf::Vector2f cameraCenter = m_CameraView.getCenter();
    double distance = std::sqrt(std::pow(m_FixedPoint.x - cameraCenter.x, 2.0) + std::pow(m_FixedPoint.x - cameraCenter.x, 2.0));

    if (distance > RECENTER_DISTANCE_THRESHOLD)
    {
        MoveCameraView((m_FixedPoint - m_CameraView.getCenter()) * RECENTER_SPEED_RATE * deltaTime);
    } 
    else 
    {
        m_CameraMode = CameraMode::FIXED;
    }
}

void CameraManager::MoveCameraView(const sf::Vector2f& offset) 
{
    m_CameraView.move(offset);
    m_HardMoveZone.setPosition(m_CameraView.getCenter() - (m_HardMoveZone.getSize() / 2.f));
    m_SoftMoveZone.setPosition(m_CameraView.getCenter() - (m_SoftMoveZone.getSize() / 2.f));
}

void CameraManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    if (DisplayCameraZones) 
    {
        target.draw(m_HardMoveZone);
        target.draw(m_SoftMoveZone);
    }
}