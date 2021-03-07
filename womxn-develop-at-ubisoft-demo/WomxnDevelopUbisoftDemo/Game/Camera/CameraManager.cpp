#include <stdafx.h>
#include "CameraManager.h"

CameraManager::CameraManager(sf::RenderWindow* window, const Player* player)
    : m_Window(window)
    , m_Player(player)
    , m_CameraView(sf::FloatRect(0.f, 0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)))
{
    m_Viewport.setPosition(window->getSize().x * 0.25f, window->getSize().y * 0.25f);
    m_Viewport.setSize({ 640.f, 320.f });
    // Temporary
    m_Viewport.setFillColor(sf::Color::Transparent);
    m_Viewport.setOutlineColor(sf::Color::Red);
    m_Viewport.setOutlineThickness(1);

    m_Window->setView(m_CameraView);
}

void CameraManager::Update(float deltaTime) 
{
    sf::FloatRect viewportBoundingBox = m_Viewport.getGlobalBounds();

    // Move the camera view towards the player if he's out of the viewport
    if (m_Player->GetCenter().x < viewportBoundingBox.left
        || m_Player->GetCenter().x > viewportBoundingBox.left + viewportBoundingBox.width
        || m_Player->GetCenter().y < viewportBoundingBox.top
        || m_Player->GetCenter().y > viewportBoundingBox.top + viewportBoundingBox.height)
    {
        m_CameraView.move((m_Player->GetCenter() - m_CameraView.getCenter()) * 0.5f * deltaTime);
        m_Viewport.setPosition(m_CameraView.getCenter() - (m_Viewport.getSize() / 2.f));
        m_Window->setView(m_CameraView);
    } 
}

// Temporary
void CameraManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(m_Viewport);
}