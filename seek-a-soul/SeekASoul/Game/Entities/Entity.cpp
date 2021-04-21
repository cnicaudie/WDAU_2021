#include <stdafx.h>
#include "Entity.h"

Entity::Entity(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position, int healthPoints)
    : m_HealthState(HealthState::OK)
    , m_TextureManager(textureManager)
    , m_Position(position)
    , m_IsGrounded(false)
    , m_HealthPoints(healthPoints)
    , m_LastDamageTime(0)
{
    SetTrigger(false);
}

