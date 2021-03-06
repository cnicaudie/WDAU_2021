#include <stdafx.h>
#include "Player.h"
#include <Game/Map/CollideableTile.h>

// Joystick helpers
namespace
{
    bool GetFirstJoystickIndex(unsigned int& index)
    {
        index = 0;
        while (index < sf::Joystick::Count)
        {
            if (sf::Joystick::isConnected(index) && sf::Joystick::hasAxis(index, sf::Joystick::Axis::X) && sf::Joystick::hasAxis(index, sf::Joystick::Axis::Y))
            {
                return true;
            }

            index++;
        }

        return false;
    }

    float GetScaledAxis(unsigned int index, sf::Joystick::Axis axis, float deadZone, float scale)
    {
        float value = (sf::Joystick::getAxisPosition(index, axis) / 100.0f) * scale;
        if (value >= -deadZone && value <= deadZone) 
        {
            return 0.0f;
        }

        return value;
    }
}


Player::Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager)
    : m_InputManager{ inputManager }
    , m_TextureManager{ textureManager }
    , m_JoystickIndex(0)
    , m_Position(50.f, 50.f)
    , m_IsGrounded(false)
    , m_CanShoot(true)
    , m_ShootCooldown(5.f)
    , m_AmmunitionsNumber(10)
    , m_Bullets{}
{
    sf::Vector2f textureSize = textureManager->GetTextureSizeFromName("PLAYER");

    m_Sprite.setTexture(textureManager->GetTextureFromName("PLAYER"));
    m_Sprite.setOrigin(textureSize * 0.5f);
    m_Sprite.setPosition(m_Position);
    m_Sprite.setScale(0.5f, 0.5f);

    SetBoundingBox(m_Position, textureSize * 0.5f);
}

void Player::Update(float deltaTime)
{
    ComputeVelocity();
    Move(deltaTime);

    UpdateShootingCooldown(deltaTime);
    
    UpdateBullets(deltaTime);

    if (m_InputManager->HasAction(Action::SHOOT) && m_CanShoot && m_AmmunitionsNumber > 0) {
        Shoot();
    }
    
    if (m_InputManager->HasAction(Action::SQUEEZE))
    {
        m_Sprite.setScale(0.25f, 0.25f);
        // TODO : Adapt the bounding box
    }
    else
    {
        m_Sprite.setScale(0.5f, 0.5f);
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Sprite);

    for (const Bullet& b : m_Bullets) {
        target.draw(b);
    }
}

void Player::OnCollision(const std::shared_ptr<BoxCollideable>& other)
{
    //std::cout << "Player had collision..." << std::endl;

    sf::FloatRect otherCollider = other->GetBoundingBox();

    if (std::dynamic_pointer_cast<CollideableTile>(other))
    {
        //std::cout << "...with CollideableTile" << std::endl;
        
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
        if (m_BoundingBox.left >= otherCollider.left + otherCollider.width
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

void Player::ComputeVelocity()
{
    const float SPEED_MAX = 200.0f;
    const float SPEED_INC = 10.0f;
    const float DEAD_ZONE = 5.0f;
    const float SLOWDOWN_RATE = 0.9f;

    const float JUMP_FORCE = 400.0f;
    
    // Compute the velocity based on the user input
    // TODO : Make some test to find a generic solution for joystick/keyboard movement
    if (m_InputManager->IsUsingJoystick())
    {
        m_Velocity.x = GetScaledAxis(m_JoystickIndex, sf::Joystick::Axis::X, DEAD_ZONE, SPEED_MAX);
        m_Velocity.y = GetScaledAxis(m_JoystickIndex, sf::Joystick::Axis::Y, DEAD_ZONE, SPEED_MAX);
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            m_Velocity.x = fmin(m_Velocity.x + SPEED_INC, SPEED_MAX);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            m_Velocity.x = fmax(m_Velocity.x - SPEED_INC, -SPEED_MAX);
        }
        else
        {
            m_Velocity.x *= SLOWDOWN_RATE;
        }
    }
    
    if (m_InputManager->HasAction(Action::JUMP) && m_IsGrounded)
    {
        m_IsGrounded = false; 
        m_Velocity.y = -JUMP_FORCE;
    }
}

void Player::Move(float deltaTime) 
{
    const float GRAVITY = 9.8f;
    m_Velocity.y += GRAVITY;
  
    // Try to move on the X axis
    sf::Vector2f tempVelocity(m_Velocity.x, 0.0f);
    if (!GameManager::GetInstance()->CheckPlayerMovement(tempVelocity * deltaTime)) 
    {
        m_Position += tempVelocity * deltaTime;
    }

    // Try to move on the Y axis
    tempVelocity.x = 0.0f;
    tempVelocity.y = m_Velocity.y;
    if (!GameManager::GetInstance()->CheckPlayerMovement(tempVelocity * deltaTime))
    {
        m_Position += tempVelocity * deltaTime;
        // Uncomment next line to avoid 1 jump when falling
        //m_IsGrounded = false;
    }

    // TODO : Either make a huge level or manage the sticking effect when trying to go out of the level
    // Uncomment next lines to clamp the player position between the bounds of the level
    //sf::Vector2u levelBounds = GameManager::GetInstance()->GetLevelBounds();
    //m_Position.x = std::clamp(m_Position.x, 0.f, static_cast<float>(levelBounds.x));
    //m_Position.y = std::clamp(m_Position.y, 0.f, static_cast<float>(levelBounds.y));

    SetCenter(m_Position);
    m_Sprite.setPosition(m_Position);
}