#include <stdafx.h>
#include "Player.h"
#include "Enemy.h"
#include <Game/Map/CollideableTile.h>
#include <Game/Map/ClimbableTile.h>
#include <Game/Objects/SoulChunk.h>

Player::Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager)
    : Entity(textureManager, { 50.f, 50.f }, 200, 1.0f)
    , Animated({ 32, 56 }, textureManager->GetTextureFromName("PLAYER_SHEET"))
    , m_InputManager{ inputManager }
    , m_CurrentState(PlayerState::IDLE)
    , m_Bullets{}
    , m_CanShoot(true)
    , m_ShootCooldown(5.f)
    , m_AmmunitionsNumber(10)
    , m_SoulChunksCollected(0)
    , m_IsClimbing(false)
    , m_IsSkullRolling(false)
    , m_SkullRollingCooldown(10.f)
    , m_InGroundCollision(false)
    , m_InCeilingCollision(false)
{
    m_BoundingBox = GetAnimatedSpriteBoundingBox();
}

void Player::Update(float deltaTime)
{
    // Check for actions
    if (m_InputManager->HasAction(Action::SHOOT) && m_CanShoot && m_AmmunitionsNumber > 0 && !m_IsSkullRolling) {
        Shoot();
    }

    if (m_InputManager->HasAction(Action::SKULL_ROLL))
    {
        if (m_SkullRollingCooldown >= 5.f) 
        {
            m_IsSkullRolling = true;
            m_SkullRollingCooldown = 0.f;
        } 
        else if (m_IsSkullRolling && m_SkullRollingCooldown >= 1.f)
        {
            m_IsSkullRolling = false;
            m_SkullRollingCooldown = 0.f;
        }
    }

    // Update player's bounding box
    UpdateBoundingBox();

    Move(deltaTime);

    // If in ground and ceiling after collision check, player stays skull rolling
    if (!m_IsSkullRolling && m_InGroundCollision && m_InCeilingCollision)
    {
        m_IsSkullRolling = true;
        m_SkullRollingCooldown = 0.f;
    }
 
    // Update Cooldowns
    UpdateDamageCooldown(deltaTime);
    UpdateShootingCooldown(deltaTime);    
    UpdateSkullRollCooldown(deltaTime);

    // Update bullets and check for impacts
    ManageBullets(deltaTime);

    // Update player's animation
    ComputeNextPlayerState();
    PlayAnimation(m_CurrentState);
}

void Player::OnCollision(BoxCollideable* other)
{
    sf::FloatRect otherCollider = other->GetBoundingBox();
    
    if (typeid(*other) == typeid(class Enemy) && !m_WasDamaged && !m_IsSkullRolling)
    {
        Damage();
    }

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

        // === Special checks for skull roll
        // (Getting out of skull roll action can cause being in ceiling and/or in ground)

        else if (m_BoundingBox.top < otherCollider.top + otherCollider.height
            && m_BoundingBox.top > otherCollider.top)
        {
            if (!m_InCeilingCollision)
            {
                m_InCeilingCollision = true;
                m_Velocity.y = 0.f;

                if (!m_InGroundCollision)
                {
                    m_Position.y = otherCollider.top + otherCollider.height + (m_BoundingBox.height / 2);
                    
                    //std::cout << "Corrected inCeiling" << std::endl;
                }

                //std::cout << "m_InCeilingCollision" << std::endl;
            }
        }
        else if (m_BoundingBox.top + m_BoundingBox.height > otherCollider.top
            && m_BoundingBox.top < otherCollider.top)
        {
            if (!m_InGroundCollision)
            {
                m_InGroundCollision = true;
                m_Velocity.y = 0.f;

                // If only in ground, reset the position of the player
                if (!m_InCeilingCollision)
                {
                    m_IsGrounded = true;
                    m_Position.y = otherCollider.top - (m_BoundingBox.height / 2);

                    //std::cout << "Corrected inGround" << std::endl;
                }

                //std::cout << "m_InGroundCollision" << std::endl;
            }
        }
    }
}

void Player::OnTrigger(BoxCollideable* other)
{
    SoulChunk* soulChunk = dynamic_cast<SoulChunk*>(other);

    if (soulChunk != nullptr && !soulChunk->WasCollected())
    {
        std::cout << "Player collected Soul Chunk" << std::endl;
        m_SoulChunksCollected += 1;
    }

    if (typeid(*other).name() == typeid(class ClimbableTile).name())
    {
        if (!m_IsClimbing && m_InputManager->HasAction(Action::MOVE_UP) && !m_IsSkullRolling)
        {
            std::cout << "Player is climbing" << std::endl;
            m_IsClimbing = true;
        } 
        // Climbed down the ladder
        else if (m_IsClimbing && m_IsGrounded) 
        {
            std::cout << "Player is not climbing anymore" << std::endl;
            m_IsClimbing = false;
        }
    }
    // Jumped out the ladder
    else if (m_IsClimbing)
    {
        std::cout << "Player is not climbing anymore" << std::endl;
        m_IsClimbing = false;

        // Player gets a little force if pressing jump key
        if (m_InputManager->HasAction(Action::MOVE_UP)) 
        {
            m_Velocity.y = -300.f;
        }
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_AnimationSprite);

    for (const Bullet& b : m_Bullets) {
        target.draw(b);
    }
}

void Player::Damage()
{
    std::cout << "Player was damaged !" << std::endl;
    m_AnimationSprite.setColor(sf::Color::Red);
    m_DamageCooldown = 0.f;
    m_WasDamaged = true;
    m_HealthPoints -= 10;

    if (m_HealthPoints == 0)
    {
        m_IsDead = true;
        std::cout << "Player died !" << std::endl;
        // TODO : Fire event player died ?
    }
}

void Player::UpdateDamageCooldown(float deltaTime)
{
    if (m_WasDamaged)
    {
        if (m_DamageCooldown >= m_DamageCooldownRelease) {
            m_WasDamaged = false;
            m_AnimationSprite.setColor(sf::Color::White);
        }
        else
        {
            m_DamageCooldown += 1.f * deltaTime;
        }
    }
}

void Player::UpdateBoundingBox()
{
    if (m_IsSkullRolling)
    {
        SetBoundingBox(m_Position, static_cast<sf::Vector2f>(GetSpriteSize()) * 0.5f);
    }
    else
    {
        SetBoundingBox(m_Position, static_cast<sf::Vector2f>(GetSpriteSize()));
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

void Player::UpdateSkullRollCooldown(float deltaTime)
{
    if (m_SkullRollingCooldown < 5.f)
    {
        m_SkullRollingCooldown += 1.f * deltaTime;
    }
    else
    {
        m_IsSkullRolling = false;
    }
}

void Player::ManageBullets(float deltaTime)
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
    const float MOVE_SPEED_MAX = 200.0f;
    const float MOVE_SPEED_INC = 10.0f;
    const float CLIMB_SPEED = 100.0f;
    const float JUMP_FORCE = 400.0f;
    const float SLOWDOWN_RATE = 0.9f;
    const float GRAVITY = 9.8f;
    
    sf::Vector2f tempVelocity(0.f, 0.f);

    // Reset in ground/ceiling collision checks
    m_InGroundCollision = false;
    m_InCeilingCollision = false;

    // Compute player's velocity
    m_Velocity.y += GRAVITY;
    m_Velocity.x = m_InputManager->GetScaledVelocity(m_Velocity.x, MOVE_SPEED_INC, MOVE_SPEED_MAX, SLOWDOWN_RATE);

    // Resets the velocity if climbing 
    // (we don't want the player to fall down the ladder if he's not giving any input)
    if (m_IsClimbing) 
    {
        m_Velocity.y = 0.f;
    }

    if (m_InputManager->HasAction(Action::MOVE_UP))
    {
        // Jump
        if (m_IsGrounded)
        {
            m_Velocity.y = -JUMP_FORCE;
            m_IsGrounded = false;
        }
        // Climb up
        else if (m_IsClimbing)
        {
            m_Velocity.y = -CLIMB_SPEED;
        }
    }
    
    // Climb down
    if (m_InputManager->HasAction(Action::MOVE_DOWN) && m_IsClimbing)
    {
        m_Velocity.y = CLIMB_SPEED;
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
        // Uncomment next line to avoid 1 jump when falling off a platform
        //m_IsGrounded = false;
    }
    
    // Clamp the player position between the bounds of the level
    sf::Vector2u levelBounds = GameManager::GetInstance()->GetLevelBounds();
    ClampPlayerPosition(0.f, static_cast<float>(levelBounds.x), 0.f, static_cast<float>(levelBounds.y));

    // Apply new position
    SetCenter(m_Position);
    SetAnimatedSpritePosition(m_Position);
}

void Player::ClampPlayerPosition(float minBoundX, float maxBoundX, float minBoundY, float maxBoundY)
{
    if (m_BoundingBox.left < minBoundX) 
    {
        m_Position.x = minBoundX + (m_BoundingBox.width / 2);
        m_Velocity.x = 0.f;
    } 
    else if (m_BoundingBox.left + m_BoundingBox.width > maxBoundX)
    {
        m_Position.x = maxBoundX - (m_BoundingBox.width / 2);
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

void Player::ComputeNextPlayerState()
{
    const float MOVE_ANIMATION_THRESHOLD = 50.f;

    if (m_IsClimbing) 
    {
        m_CurrentState = PlayerState::CLIMBING;
    } 
    // TODO : make an idle animation for skull roll ?
    else if (m_IsSkullRolling)
    {
        m_CurrentState = PlayerState::SKULL_ROLLING;
    }
    else if (std::abs(m_Velocity.x) > MOVE_ANIMATION_THRESHOLD)
    {
        m_CurrentState = PlayerState::MOVING;
        
        FlipSprite(m_Velocity.x < 0);
    } 
    else 
    {
        m_CurrentState = PlayerState::IDLE;
    }
}