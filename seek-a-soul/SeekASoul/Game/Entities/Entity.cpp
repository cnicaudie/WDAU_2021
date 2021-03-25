#include <stdafx.h>
#include "Entity.h"

Entity::Entity(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position, int healthPoints, float damageCooldownRelease)
    : m_TextureManager(textureManager)
    , m_Position(position)
    , m_HealthPoints(healthPoints)
    , m_IsDead(false)
    , m_WasDamaged(false)
    , m_DamageCooldown(2.f)
    , m_DamageCooldownRelease(damageCooldownRelease)
{
    SetTrigger(false);
}