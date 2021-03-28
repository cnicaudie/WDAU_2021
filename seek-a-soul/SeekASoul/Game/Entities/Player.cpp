#include <stdafx.h>
#include "Player.h"
#include "Enemy.h"
#include <Game/Map/Tiles/CollideableTile.h>
#include <Game/Map/Tiles/ClimbableTile.h>
#include <Game/Objects/SoulChunk.h>
#include <Engine/Event/EventTypes/ActionEvent.h>

static constexpr uint64_t SHOOT_COOLDOWN = 500;
static constexpr uint64_t DAMAGE_COOLDOWN = 1000;
static constexpr uint64_t SKULL_ROLL_COOLDOWN = 5000;

static constexpr float MOVE_SPEED_MAX = 200.0f;
static constexpr float MOVE_SPEED_INC = 10.0f;
static constexpr float CLIMB_SPEED = 100.0f;
static constexpr float JUMP_FORCE = 400.0f;
static constexpr float SLOWDOWN_RATE = 0.9f;
static constexpr float GRAVITY = 9.8f;

Player::Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager)
    : Entity(textureManager, { 50.f, 50.f }, 200)
    , Animated({ 32, 56 }, textureManager->GetTextureFromName("PLAYER_SHEET"))
    , m_InputManager{ inputManager }
    , m_CurrentState(PlayerState::IDLE)
    , m_SoulChunksCollected(0)
    , m_JumpCount(1)
    , m_IsGrounded(false)
    , m_IsClimbing(false)
    , m_CanClimb(false)
    , m_IsSkullRolling(false)
    , m_LastSkullRollTime(0)
    , m_LastShootTime(0)
    , m_Bullets{}
    , m_AmmunitionsNumber(10) // TODO : Display this number with UI
    , m_InGroundCollision(false)
    , m_InCeilingCollision(false)
{
    m_BoundingBox = GetAnimatedSpriteBoundingBox();

    EventListener<Player, ActionEvent> listener(this, &Player::OnEvent);
    EventManager::GetInstance()->AddListener(listener);
}

void Player::Update(float deltaTime)
{
    // TODO : make a unique function to get this value
    auto time = std::chrono::system_clock::now().time_since_epoch();
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();

    if (m_IsSkullRolling) 
    {
        UpdateSkullRollCooldown(now);
    }
    else if (m_InGroundCollision && m_InCeilingCollision)
    {
        // If in ground and ceiling after collision check, player stays skull rolling
        LOG_DEBUG("SKULL ROLL STAY");
        m_IsSkullRolling = true;
        m_LastSkullRollTime = now;
    }

    // Update player's bounding box
    UpdateBoundingBox();

    Move(deltaTime);

    if (m_HealthState == HealthState::DAMAGED)
    {
        UpdateVisualDamage(now);
    }

    // Update bullets and check for impacts
    ManageBullets(deltaTime);

    // Update player's animation
    ComputeNextPlayerState();
    PlayAnimation(static_cast<int>(m_CurrentState));
}

void Player::OnEvent(const Event* evnt) 
{
    if (const ActionEvent* otherActionEvent = dynamic_cast<const ActionEvent*>(evnt))
    {
        switch(otherActionEvent->GetActionType()) 
        {
            case Action::SHOOT: 
            {
                Shoot();
                break;
            }

            case Action::SKULL_ROLL:
            {
                SkullRoll();
                break;
            }

            default:
                break;
        }
    }
}

void Player::OnCollision(BoxCollideable* other)
{
    sf::FloatRect otherCollider = other->GetBoundingBox();
    
    if (typeid(*other) == typeid(class Enemy) 
        && (m_HealthState == HealthState::OK)
        && !m_IsSkullRolling)
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
            m_JumpCount = 1;
            m_Position.y = otherCollider.top - (m_BoundingBox.height / 2);
            //LOG_DEBUG("Bottom collision");
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
                    m_JumpCount = 1;
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
    Collectible* collectible = dynamic_cast<Collectible*>(other);

    if (collectible != nullptr && !collectible->WasCollected())
    {
        if (typeid(*collectible).name() == typeid(class SoulChunk).name()) 
        {
            LOG_INFO("Player collected SoulChunk !");
            m_SoulChunksCollected += 1;
        }
    }

    if (typeid(*other).name() == typeid(class ClimbableTile).name())
    {
        m_CanClimb = true;
    }
    else 
    {
        m_CanClimb = false;
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_AnimationSprite);

    for (const Bullet& b : m_Bullets) {
        target.draw(b);
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

void Player::Move(float deltaTime)
{
    sf::Vector2f tempVelocity(0.f, 0.f);

    // Reset in ground/ceiling collision checks
    m_InGroundCollision = false;
    m_InCeilingCollision = false;

    // Compute player's velocity
    m_Velocity.y += GRAVITY;
    m_Velocity.x = m_InputManager->GetScaledVelocity(m_Velocity.x, MOVE_SPEED_INC, MOVE_SPEED_MAX, SLOWDOWN_RATE);

    if (m_InputManager->HasAction(Action::MOVE_UP))
    {
        MoveUp();
    } 
    else if (m_IsClimbing) 
    {
        // Resets the vertical velocity if climbing 
        // (we don't want the player to fall down the ladder if he's not giving any input)
        m_Velocity.y = 0.f;

        if (!m_CanClimb) 
        {
            m_IsClimbing = false;
        }
    }

    // Climb down
    if (m_InputManager->HasAction(Action::MOVE_DOWN) && m_IsClimbing)
    {
        m_Velocity.y = CLIMB_SPEED;

        if (m_IsGrounded)
        {
           m_IsClimbing = false;
        }
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
    }

    // Clamp the player position between the bounds of the level
    sf::Vector2u levelBounds = GameManager::GetInstance()->GetLevelBounds();
    ClampPlayerPosition(0.f, static_cast<float>(levelBounds.x), 0.f, static_cast<float>(levelBounds.y));

    // Apply new position
    SetCenter(m_Position);
    SetAnimatedSpritePosition(m_Position);
}

void Player::MoveUp()
{
    // Climb up
    if (m_CanClimb && !m_IsSkullRolling)
    {
        //LOG_INFO("Player is climbing !");
        m_Velocity.y = -CLIMB_SPEED;
        m_IsClimbing = true;
        m_JumpCount = 0;
    }
    else if (!m_CanClimb && m_IsClimbing)
    {
        // Player gets a little force if he's jumping when getting out of the ladder
        m_Velocity.y = -(JUMP_FORCE * 0.75f);
        m_IsClimbing = false;
    }
    // Jump
    else if (m_JumpCount != 0)
    {
        m_JumpCount -= 1;
        m_Velocity.y = -JUMP_FORCE;
    }
    
    m_IsGrounded = false;
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
        m_JumpCount = 1;
        m_Position.y = maxBoundY - (m_BoundingBox.height / 2);
        m_Velocity.y = 0.f;
    }
}

void Player::Damage()
{
    LOG_INFO("Player was damaged !");

    m_AnimationSprite.setColor(sf::Color::Red);
    m_HealthState = HealthState::DAMAGED;
    m_HealthPoints -= 10;

    if (m_HealthPoints == 0)
    {
        m_HealthState = HealthState::DEAD;
        LOG_INFO("Player died !");
        // TODO : Fire event player died ?
    }
}

void Player::UpdateVisualDamage(uint64_t now)
{
    if ((now - m_LastDamageTime) >= DAMAGE_COOLDOWN) 
    {
        m_AnimationSprite.setColor(sf::Color::White);
        m_HealthState = HealthState::OK;
        m_LastDamageTime = now;
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

void Player::Shoot()
{
    // TODO : make a unique function to get this value
    auto time = std::chrono::system_clock::now().time_since_epoch();
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();

    if ((now - m_LastShootTime) >= SHOOT_COOLDOWN
        && m_AmmunitionsNumber > 0
        && !m_IsSkullRolling) 
    {
        const sf::Vector2f bulletDirection = m_InputManager->GetScaledShootDirection(m_Position);

        m_Bullets.emplace_back(m_TextureManager, bulletDirection, m_Position);

        m_AmmunitionsNumber--;
        LOG_INFO("Ammunitions left : " << m_AmmunitionsNumber);

        m_LastShootTime = now;
    }
}

void Player::SkullRoll()
{
    // TODO : make a unique function to get this value
    auto time = std::chrono::system_clock::now().time_since_epoch();
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
    
    if (m_IsSkullRolling && (now - m_LastSkullRollTime) >= (SKULL_ROLL_COOLDOWN * 0.25f))
    {
        LOG_DEBUG("SKULL ROLL OUT BY INPUT");
        m_IsSkullRolling = false;
        m_LastSkullRollTime = now;
    }
    else if ((now - m_LastSkullRollTime) >= SKULL_ROLL_COOLDOWN)
    {
        LOG_DEBUG("SKULL ROLL IN");
        m_IsSkullRolling = true;
        m_LastSkullRollTime = now;
    }
}

void Player::UpdateSkullRollCooldown(uint64_t now)
{
    if ((now - m_LastSkullRollTime) >= SKULL_ROLL_COOLDOWN)
    {
        LOG_DEBUG("SKULL ROLL OUT");
        m_IsSkullRolling = false;
        m_LastSkullRollTime = now;
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