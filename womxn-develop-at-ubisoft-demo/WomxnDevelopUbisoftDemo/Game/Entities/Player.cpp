#include <stdafx.h>
#include "Player.h"
#include <Game/Map/CollideableTile.h>

Player::Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager)
    : Entity(textureManager, { 50.f, 50.f }, 200)
    , m_InputManager{ inputManager }
    , m_CanShoot(true)
    , m_ShootCooldown(5.f)
    , m_AmmunitionsNumber(10)
    , m_Bullets{}
{
    sf::Vector2f textureSize = textureManager->GetTextureSizeFromName("PLAYER");

    m_Sprite.setTexture(textureManager->GetTextureFromName("PLAYER"));
    m_Sprite.setOrigin(textureSize * 0.5f);
    m_Sprite.setPosition(m_Position);
    //m_Sprite.setScale(0.5f, 0.5f);

    SetBoundingBox(m_Position, textureSize);
}

void Player::Update(float deltaTime)
{
    Move(deltaTime);

    UpdateShootingCooldown(deltaTime);
    
    UpdateBullets(deltaTime);

    if (m_InputManager->HasAction(Action::SHOOT) && m_CanShoot && m_AmmunitionsNumber > 0) {
        Shoot();
    }
    
    // TODO : Manage the crouch/squeeze feature differently (scaling is not ideal)
    if (m_InputManager->HasAction(Action::SQUEEZE))
    {
        m_Sprite.setScale(0.8f, 0.8f);
        sf::Vector2f textureSize = m_TextureManager->GetTextureSizeFromName("PLAYER");
        SetBoundingBox(m_Position, textureSize * 0.8f);
    } 
    else
    {
        m_Sprite.setScale(1.f, 1.f);
        sf::Vector2f textureSize = m_TextureManager->GetTextureSizeFromName("PLAYER");
        SetBoundingBox(m_Position, textureSize);
    }
}

void Player::OnCollision(const BoxCollideable* other)
{
    sf::FloatRect otherCollider = other->GetBoundingBox();

    if (typeid(*other) == typeid(class CollideableTile))
    {
        // Bottom collision
        if (m_BoundingBox.top + m_BoundingBox.height <= otherCollider.top
            && m_BoundingBox.top < otherCollider.top)
        {
            m_Velocity.y = 0.f;
            m_IsGrounded = true;
            m_Position.y = otherCollider.top - (m_BoundingBox.height / 2);
            //std::cout << "Bottom collision" << std::endl;
        }

        // Top collision
        else if (m_BoundingBox.top >= otherCollider.top + otherCollider.height
            && m_BoundingBox.top + m_BoundingBox.height > otherCollider.top + otherCollider.height)
        {
            m_Velocity.y = 0.f;
            m_Position.y = otherCollider.top + otherCollider.height + (m_BoundingBox.height / 2);
            //std::cout << "Top collision" << std::endl;
        }

        // Left collision
        else if (m_BoundingBox.left >= otherCollider.left + otherCollider.width
            && m_BoundingBox.left + m_BoundingBox.width > otherCollider.left + otherCollider.width)
        {
            m_Velocity.x = 0.f;
            m_Position.x = otherCollider.left + otherCollider.width + (m_BoundingBox.width / 2);
            //std::cout << "Left collision" << std::endl;
        }

        // Right collision
        else if (m_BoundingBox.left + m_BoundingBox.width <= otherCollider.left
            && m_BoundingBox.left < otherCollider.left)
        {
            m_Velocity.x = 0.f;
            m_Position.x = otherCollider.left - (m_BoundingBox.width / 2);
            //std::cout << "Right collision" << std::endl;
        }
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    Entity::draw(target, states);

    for (const Bullet& b : m_Bullets) {
        target.draw(b);
    }
}

void Player::UpdateShootingCooldown(float deltaTime)
{
    if (m_ShootCooldown >= 0.5f) {
        m_CanShoot = true;
    }
    else {
        m_ShootCooldown += 1.f * deltaTime;
    }
}

void Player::UpdateBullets(float deltaTime)
{
    // Update the bullets
    for (Bullet& b : m_Bullets) {
        b.Update(deltaTime);
    }

    // Check for impact
    int bulletIndex = 0;
    for (Bullet& b : m_Bullets) 
    {
        if (b.GetDistance() > 400.f || b.HadImpact()) {
            m_Bullets.erase(m_Bullets.begin() + bulletIndex);
        }
        bulletIndex++;
    }
}

void Player::Shoot()
{
    const sf::Vector2f mousePos = m_InputManager->GetMousePosition();
    const sf::Vector2f bulletDirection = mousePos - m_Position;
    
    // TODO : Make a normalize function in a MathUtils file
    float magnitude = std::sqrt(bulletDirection.x * bulletDirection.x + bulletDirection.y * bulletDirection.y);
    sf::Vector2f normalizedBulletDirection = bulletDirection / magnitude;
    
    m_CanShoot = false;
    m_AmmunitionsNumber--;
    std::cout << "Ammunitions left : " << m_AmmunitionsNumber << std::endl;
    m_ShootCooldown = 0.f;
    m_Bullets.emplace_back(m_TextureManager, normalizedBulletDirection, m_Position);
}

void Player::Move(float deltaTime) 
{
    const float SPEED_MAX = 200.0f;
    const float SPEED_INC = 10.0f;
    const float DEAD_ZONE = 5.0f;
    const float SLOWDOWN_RATE = 0.9f;
    const float GRAVITY = 9.8f;
    sf::Vector2f tempVelocity(0.f, 0.f);

    // Compute player's velocity
    m_Velocity.y += GRAVITY;
    m_Velocity.x = m_InputManager->GetScaledVelocity(m_Velocity.x, SPEED_INC, SPEED_MAX, SLOWDOWN_RATE, DEAD_ZONE);

    if (m_InputManager->HasAction(Action::JUMP) && m_IsGrounded)
    {
        m_IsGrounded = false;
        m_Velocity.y = -400.0f;
    }

    // Check movement on X axis
    tempVelocity.x = m_Velocity.x;
    if (!GameManager::GetInstance()->CheckCollision(this, tempVelocity * deltaTime)) 
    {
        m_Position += tempVelocity * deltaTime;
    }

    // Check movement on Y axis
    tempVelocity.x = 0.0f;
    tempVelocity.y = m_Velocity.y;
    if (!GameManager::GetInstance()->CheckCollision(this, tempVelocity * deltaTime))
    {
        m_Position += tempVelocity * deltaTime;
        // Uncomment next line to avoid 1 jump when falling
        //m_IsGrounded = false;
    }
    
    // Clamp the player position between the bounds of the level
    sf::Vector2u levelBounds = GameManager::GetInstance()->GetLevelBounds();
    ClampPlayerPosition(0.f, static_cast<float>(levelBounds.x), 0.f, static_cast<float>(levelBounds.y));

    // Apply new position
    SetCenter(m_Position);
    m_Sprite.setPosition(m_Position);
}

void Player::ClampPlayerPosition(float minBoundX, float maxBoundX, float minBoundY, float maxBoundY)
{
    if (m_BoundingBox.left < minBoundX) 
    {
        m_Position.x = minBoundX + (m_BoundingBox.height / 2);
        m_Velocity.x = 0.f;
    } 
    else if (m_BoundingBox.left + m_BoundingBox.width > maxBoundX)
    {
        m_Position.x = maxBoundX - (m_BoundingBox.height / 2);
        m_Velocity.x = 0.f;
    } 
    else if (m_BoundingBox.top < minBoundY)
    {
        m_Position.y = minBoundY + (m_BoundingBox.height / 2);
        m_Velocity.y = 0.f;
    }
    else if (m_BoundingBox.top + m_BoundingBox.height > maxBoundY)
    {
        m_Position.y = maxBoundY - (m_BoundingBox.height / 2);
        m_Velocity.y = 0.f;
    }
}
