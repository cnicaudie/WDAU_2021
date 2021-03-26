#include <stdafx.h>
#include "CameraManager.h"

CameraManager::CameraManager(sf::RenderWindow* window)
    : m_Window(window)
    , m_CameraView(sf::FloatRect(0.f, 0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)))
    , m_ExitedHardZone(false)
    , DisplayCameraZones(false)
{
    const sf::Vector2f WINDOW_CENTER{ window->getSize().x * 0.25f, window->getSize().y * 0.25f };
    const sf::Vector2f HARD_ZONE_SIZE{ window->getSize().x / 1.6f, window->getSize().y / 2.4f };
    const sf::Vector2f SOFT_ZONE_SIZE{ window->getSize().x / 3.2f, window->getSize().y / 4.8f };

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
    if (m_CameraMode == CameraMode::FOLLOW) 
    {
        FollowBox(deltaTime);
    }
}

void CameraManager::FollowBox(float deltaTime)
{
    const sf::Vector2f boxToFollowPosition = m_BoxToFollow->GetCenter();

    sf::FloatRect hardMoveZoneBoundingBox = m_HardMoveZone.getGlobalBounds();
    sf::FloatRect softMoveZoneBoundingBox = m_SoftMoveZone.getGlobalBounds();

    float followSpeedRate = 1.f;

    if (!hardMoveZoneBoundingBox.contains(boxToFollowPosition))
    {
        m_ExitedHardZone = true;
    } 
    else if (m_ExitedHardZone)
    {
        if (softMoveZoneBoundingBox.contains(boxToFollowPosition))
        {
            m_ExitedHardZone = false;
            followSpeedRate = 0.5f;
        }
    } 
    else if (!softMoveZoneBoundingBox.contains(boxToFollowPosition))
    {
        followSpeedRate = 0.5f;
    }
    else 
    {
        followSpeedRate = 0.25f;
    }

    m_CameraView.move((boxToFollowPosition - m_CameraView.getCenter()) * followSpeedRate * deltaTime);
    
    m_HardMoveZone.setPosition(m_CameraView.getCenter() - (m_HardMoveZone.getSize() / 2.f));
    m_SoftMoveZone.setPosition(m_CameraView.getCenter() - (m_SoftMoveZone.getSize() / 2.f));
    m_Window->setView(m_CameraView);
}

void CameraManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    if (DisplayCameraZones) 
    {
        target.draw(m_HardMoveZone);
        target.draw(m_SoftMoveZone);
    }
}