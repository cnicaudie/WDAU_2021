#include <stdafx.h>
#include "Player.h"

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


Player::Player(const TextureManager& textureManager)
    : m_TextureManager{ textureManager }
    , m_IsUsingJoystick(false)
    , m_JoystickIndex(0)
    , m_WasButtonPressed(false)
    , m_Position(250.0f, 250.0f)
    , m_IsGrounded(false)
    , m_canShoot(true)
    , m_shootCooldown(5.f)
    , m_AmmunitionsNumber(10)
    , m_Bullets{}
{
    sf::Vector2f textureSize = textureManager.GetTextureSizeFromName("PLAYER");

    m_Sprite.setTexture(textureManager.GetTextureFromName("PLAYER"));
    m_Sprite.setOrigin(textureSize * 0.5f);
    m_Sprite.setPosition(m_Position);

    SetBoundingBox(m_Position, textureSize);

    m_IsUsingJoystick = GetFirstJoystickIndex(m_JoystickIndex);
}

void Player::Update(float deltaTime)
{
    ComputeVelocity();

    Move(deltaTime);

    UpdateShootingCooldown(deltaTime);
    
    UpdateBullets(deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m_canShoot && m_AmmunitionsNumber > 0) {
        Shoot();
    }
    
    // Reduces scale when pressing space (crouch feature ?)
    // TODO : Clean that
    if (m_IsUsingJoystick) {
        if (sf::Joystick::isButtonPressed(m_JoystickIndex, 0))
        {
            if (!m_WasButtonPressed)
            {
                m_Sprite.setScale(0.8f, 0.8f);
                m_WasButtonPressed = true;
            }
        }
        else
        {
            if (m_WasButtonPressed)
            {
                m_Sprite.setScale(1.0f, 1.0f);
                m_WasButtonPressed = false;
            }
        }
    }
    else 
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (!m_WasButtonPressed)
            {
                m_Sprite.setScale(0.8f, 0.8f);
                m_WasButtonPressed = true;
            }
        }
        else
        {
            if (m_WasButtonPressed)
            {
                m_Sprite.setScale(1.0f, 1.0f);
                m_WasButtonPressed = false;
            }
        }
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Sprite);

    for (const Bullet& b : m_Bullets) {
        target.draw(b);
    }
}

void Player::UpdateShootingCooldown(float deltaTime)
{
    if (m_shootCooldown >= 0.5f) {
        m_canShoot = true;
    }
    else {
        m_shootCooldown += 1.f * deltaTime;
    }
}

void Player::UpdateBullets(float deltaTime)
{
    // Erase bullets if they crossed a certain distance
    int bulletNumber = 0;
    for (Bullet& b : m_Bullets) {
        if (b.GetDistance() > 400.f) {
            m_Bullets.erase(m_Bullets.begin() + bulletNumber);
        }
        bulletNumber++;
    }

    // Update the bullets
    for (Bullet& b : m_Bullets) {
        b.Update(deltaTime);
    }
}

void Player::Shoot()
{
    m_canShoot = false;
    m_AmmunitionsNumber--;
    m_shootCooldown = 0.f;
    m_Bullets.emplace_back(m_TextureManager, sf::Vector2f(1.f, 0.f), m_Position);
}

void Player::ComputeVelocity()
{
    const float SPEED_MAX = 200.0f;
    const float SPEED_INC = 10.0f;
    const float DEAD_ZONE = 5.0f;
    const float SLOWDOWN_RATE = 0.9f;

    const float JUMP_FORCE = 400.0f;
    
    // Compute the velocity based on the user input
    if (m_IsUsingJoystick)
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_IsGrounded) // Jump
        {
            m_Velocity.y = -JUMP_FORCE;
            m_IsGrounded = false;
        }
    }
}

void Player::Move(float deltaTime) 
{
    const float GRAVITY = 9.8f;

    // Apply gravity
    m_Velocity.y += GRAVITY;

    // Try to move on the X axis
    sf::Vector2f tempVelocity(m_Velocity.x, 0.0f);
    if (!CheckCollision(tempVelocity * deltaTime)) 
    {
        m_Position += tempVelocity * deltaTime;
    }

    // Try to move on the Y axis
    tempVelocity.x = 0.0f;
    tempVelocity.y = m_Velocity.y;
    if (!CheckCollision(tempVelocity * deltaTime)) 
    {
        m_Position += tempVelocity * deltaTime;
        // Uncomment next line to avoid 1 jump when falling
        //m_IsGrounded = false;
    }

    SetCenter(m_Position);
    m_Sprite.setPosition(m_Position);
}

bool Player::CheckCollision(const sf::Vector2f& nextPosition) {
    bool isColliding = false;

    sf::FloatRect otherCollider;

    // Set up the player's future bounding box
    sf::FloatRect playerCollider = m_BoundingBox;
    playerCollider.left += nextPosition.x;
    playerCollider.top += nextPosition.y;

    // Search for a collision
    for (Wall w : m_Walls) 
    {
        otherCollider = w.GetBoundingBox();

        if (playerCollider.intersects(otherCollider))
        {
            isColliding = true;

            // Check the direction of the collision 

            // Bottom collision
            if (playerCollider.top < otherCollider.top && playerCollider.top + playerCollider.height < otherCollider.top +  otherCollider.height)
            {
                m_IsGrounded = true;
                m_Velocity.y = 0.f;
                m_Position.y = otherCollider.top - (m_BoundingBox.height / 2);
            }

            // Top collision
            else if (playerCollider.top > otherCollider.top && playerCollider.top + playerCollider.height > otherCollider.top + otherCollider.height)
            {
                m_Velocity.y = 0.f;
                m_Position.y = otherCollider.top + otherCollider.height + (m_BoundingBox.height / 2) + 0.5f;
            }

            // Right collision
            else if (playerCollider.left < otherCollider.left && playerCollider.left + playerCollider.width < otherCollider.left + otherCollider.width) 
            {
                m_Velocity.x = 0.f;
                m_Position.x = otherCollider.left - (m_BoundingBox.width / 2) - 0.5f;
            }
            
            // Left collision
            else if (playerCollider.left > otherCollider.left && playerCollider.left + playerCollider.width > otherCollider.left + otherCollider.width) 
            {
                m_Velocity.x = 0.f;
                m_Position.x = otherCollider.left + otherCollider.width + (m_BoundingBox.width / 2) + 0.5f;
            }
        }
    }
    
    return isColliding;
}

