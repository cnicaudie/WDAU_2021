#include <stdafx.h>
#include "CameraManager.h"

CameraManager::CameraManager(sf::RenderWindow* window, const Player* player)
    : m_Window(window)
    , m_Player(player)
    , m_CameraView(sf::FloatRect(0.f, 0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)))
    , DisplayCameraZones(false)
{
    m_HardMoveZone.setPosition(window->getSize().x * 0.25f, window->getSize().y * 0.25f);
    m_SoftMoveZone.setPosition(window->getSize().x * 0.25f, window->getSize().y * 0.25f);

    m_HardMoveZone.setSize({ 640.f, 320.f });
    m_SoftMoveZone.setSize({ 320.f, 160.f });

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

    // Move the camera view towards the player if he's out of the zone(s)
    if (m_Player->GetCenter().x < hardMoveZoneBoundingBox.left
        || m_Player->GetCenter().x > hardMoveZoneBoundingBox.left + hardMoveZoneBoundingBox.width
        || m_Player->GetCenter().y < hardMoveZoneBoundingBox.top
        || m_Player->GetCenter().y > hardMoveZoneBoundingBox.top + hardMoveZoneBoundingBox.height)
    {
        m_CameraView.move((m_Player->GetCenter() - m_CameraView.getCenter()) * deltaTime);
    }
    else if (m_Player->GetCenter().x < softMoveZoneBoundingBox.left
        || m_Player->GetCenter().x > softMoveZoneBoundingBox.left + softMoveZoneBoundingBox.width
        || m_Player->GetCenter().y < softMoveZoneBoundingBox.top
        || m_Player->GetCenter().y > softMoveZoneBoundingBox.top + softMoveZoneBoundingBox.height)
    {
        m_CameraView.move((m_Player->GetCenter() - m_CameraView.getCenter()) * 0.5f * deltaTime);
    } 
    else 
    { 
        m_CameraView.move((m_Player->GetCenter() - m_CameraView.getCenter()) * 0.25f * deltaTime);
    }
    
    m_SoftMoveZone.setPosition(m_CameraView.getCenter() - (m_SoftMoveZone.getSize() / 2.f));
    m_HardMoveZone.setPosition(m_CameraView.getCenter() - (m_HardMoveZone.getSize() / 2.f));
    m_Window->setView(m_CameraView);
}

// Temporary
void CameraManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    if (DisplayCameraZones) 
    {
        target.draw(m_HardMoveZone);
        target.draw(m_SoftMoveZone);
    }
}