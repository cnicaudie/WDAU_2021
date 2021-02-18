#include <stdafx.h>
#include <Game/MainCharacter.h>
#include <iostream>

using namespace sf;

// Joystick helpers
namespace
{
    bool GetFirstJoystickIndex(unsigned int& index)
    {
        index = 0;
        while (index < Joystick::Count)
        {
            if (Joystick::isConnected(index) && Joystick::hasAxis(index, Joystick::Axis::X) && Joystick::hasAxis(index, Joystick::Axis::Y))
                return true;

            index++;
        }

        return false;
    }

    float GetScaledAxis(unsigned int index, sf::Joystick::Axis axis, float deadZone, float scale)
    {
        float value = (Joystick::getAxisPosition(index, axis) / 100.0f) * scale;
        if (value >= -deadZone && value <= deadZone)
            return 0.0f;

        return value;
    }
}


MainCharacter::MainCharacter()
    : m_IsPlayingEndGame(false), m_Position(250.0f, 250.0f), m_IsUsingJoystick(false), m_JoystickIndex(0), m_WasButtonPressed(false)
    , m_IsGrounded(false), m_IsJumping(false)
{
    m_Texture.loadFromFile(".\\Assets\\red_ball.bmp");

    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

    m_Sprite.setTexture(m_Texture);
    m_Sprite.setOrigin(size * 0.5f);
    m_Sprite.setPosition(m_Position);

    SetBoundingBox(m_Position, size);

    m_IsUsingJoystick = GetFirstJoystickIndex(m_JoystickIndex);
}


void MainCharacter::Update(float deltaTime)
{
    if (m_IsPlayingEndGame)
    {
        return;
    }

    ComputeVelocity();

    Move(deltaTime);
    
    // Reduces scale when pressing space (crouch feature ?)
    if (Keyboard::isKeyPressed(Keyboard::Space))
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

void MainCharacter::ComputeVelocity()
{
    const float SPEED_MAX = 150.0f;
    const float SPEED_INC = 10.0f;
    const float DEAD_ZONE = 5.0f;
    const float SLOWDOWN_RATE = 0.9f;

    const float JUMP_FORCE = 400.0f;
    
    // Compute the velocity based on the user input
    if (m_IsUsingJoystick)
    {
        m_Velocity.x = GetScaledAxis(m_JoystickIndex, Joystick::Axis::X, DEAD_ZONE, SPEED_MAX);
        m_Velocity.y = GetScaledAxis(m_JoystickIndex, Joystick::Axis::Y, DEAD_ZONE, SPEED_MAX);

        if (Joystick::isButtonPressed(m_JoystickIndex, 0))
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
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            m_Velocity.x = fmin(m_Velocity.x + SPEED_INC, SPEED_MAX);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            m_Velocity.x = fmax(m_Velocity.x - SPEED_INC, -SPEED_MAX);
        }
        else
        {
            m_Velocity.x *= SLOWDOWN_RATE;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            m_Velocity.y = fmin(m_Velocity.y + SPEED_INC, SPEED_MAX);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up) && !m_IsJumping) // Jump
        {
            m_Velocity.y = -JUMP_FORCE;
            m_IsGrounded = false;
            m_IsJumping = true;
        }
    }
}

void MainCharacter::Move(float deltaTime) 
{
    const float GRAVITY = 9.8f;

    // Apply gravity
    if (!m_IsGrounded) {
        m_Velocity.y += GRAVITY;
    }

    // Keep the oldPosition
    sf::Vector2f oldPosition = m_Position;
    
    // Try to move on the X axis
    sf::Vector2f tempVelocity(m_Velocity.x, 0.0f);
    if (!CheckCollision(m_Position + tempVelocity * deltaTime)) 
    {
        m_Position += tempVelocity * deltaTime;
    }
    else // Collided with wall
    {
        m_Velocity.x = 0.0f; // Reset horizontal velocity
    }

    // Try to move on the Y axis
    tempVelocity.x = 0.0f;
    tempVelocity.y = m_Velocity.y;
    if (!CheckCollision(m_Position + tempVelocity * deltaTime)) 
    {
        m_Position += tempVelocity * deltaTime;
    } 
    else if (tempVelocity.y < 0) // Collided with ceiling
    {
        m_Velocity.y = 0.0f; // Reset vertical velocity
    }
    else // Collided with ground
    {
        m_IsGrounded = true;
        m_IsJumping = false;
    }

    SetCenter(m_Position);
    m_Sprite.setPosition(m_Position);
}

class Dummy : public BoxCollideable {
public:
    Dummy(const sf::Vector2f& position, const sf::Vector2f& size) {
        SetBoundingBox(position, size);
    }
};

bool MainCharacter::CheckCollision(const sf::Vector2f& nextPosition) {
    sf::Vector2f size(m_BoundingBox.width, m_BoundingBox.height);
    Dummy d(nextPosition, size);

    bool isColliding = false;
    for (Wall w : m_Walls) {
        if (w.IsColliding(d)) {
            isColliding = true;
        }
    }
    return isColliding;
}

void MainCharacter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Sprite);
}

void MainCharacter::StartEndGame()
{
    m_IsPlayingEndGame = true;
}
