#include <stdafx.h>
#include "Entity.h"

Entity::Entity(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position, int healthPoints)
    : m_TextureManager(textureManager)
    , m_Position(position)
    , m_HealthPoints(healthPoints)
    , m_IsDead(false)
    , m_WasDamaged(false)
    , m_LastDamageTime(0)
{
    SetTrigger(false);
}

