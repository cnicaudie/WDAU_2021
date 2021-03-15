#include <stdafx.h>
#include "Entity.h"
#include <Game/Map/CollideableTile.h>

Entity::Entity(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position, int healthPoints, float damageCooldownRelease)
    : m_TextureManager(textureManager)
    , m_Position(position)
    , m_IsGrounded(false)
    , m_HealthPoints(healthPoints)
    , m_IsDead(false)
    , m_WasDamaged(false)
    , m_DamageCooldown(2.f)
    , m_DamageCooldownRelease(damageCooldownRelease)
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

void Entity::UpdateDamageCooldown(float deltaTime)
{
    if (m_WasDamaged)
    {
        if (m_DamageCooldown >= m_DamageCooldownRelease) {
            m_WasDamaged = false;
            m_Sprite.setColor(sf::Color::White);
        }
        else
        {
            m_DamageCooldown += 1.f * deltaTime;
        }
    }
}