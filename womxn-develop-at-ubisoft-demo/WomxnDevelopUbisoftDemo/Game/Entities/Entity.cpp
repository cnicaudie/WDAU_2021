#include <stdafx.h>
#include "Entity.h"
#include <Game/Map/CollideableTile.h>

Entity::Entity(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position, int healthPoints)
    : m_TextureManager(textureManager)
    , m_Position(position)
    , m_IsGrounded(false)
    , m_HealthPoints(healthPoints)
    , m_IsDead(false)
{
    SetTrigger(false);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_IsDead)
    {
        target.draw(m_Sprite);
    }
}