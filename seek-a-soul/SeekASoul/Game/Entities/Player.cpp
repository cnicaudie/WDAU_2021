#include <stdafx.h>
#include "Player.h"
#include "Enemy.h"
#include <Game/Map/Tiles/CollideableTile.h>
#include <Game/Map/Tiles/ClimbableTile.h>
#include <Game/Objects/SoulChunk.h>
#include <Engine/Event/EventTypes/ActionEvent.h>

static const sf::Vector2i PLAYER_SPRITE_SIZE{ 32, 56 };

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
    , Animated(PLAYER_SPRITE_SIZE, textureManager->GetTextureFromName("PLAYER_SHEET"))
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
    , m_ShootDirection{ 0.f, 0.f }
    , m_Bullets{}
    , m_AmmunitionsNumber(10) // TODO : Display this number with UI
    , m_InGroundCollision(false)
    , m_InCeilingCollision(false)
{
    m_BoundingBox = GetAnimatedSpriteBoundingBox();

    UIViewModel::GetInstance()->SetAmmunitionsNumber(m_AmmunitionsNumber);

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
    
    // Update player's bounding box
    UpdateBoundingBox();

    Move(deltaTime);
    
    if (m_InGroundCollision && m_InCeilingCollision)
    {
        // If in ground and ceiling after collision check, player stays skull rolling
        LOG_DEBUG("SKULL ROLL STAY");
        m_IsSkullRolling = true;
        m_LastSkullRollTime = now;
    }

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
    if (const ActionEvent* actionEvent = dynamic_cast<const ActionEvent*>(evnt))
    {
        switch (actionEvent->GetActionType()) 
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

            case Action::MOVE_UP:
            {
                MoveUp();
                break;
            }

            case Action::MOVE_DOWN:
            {
                MoveDown();
                break;
            }

            case Action::MOVE_RIGHT:
            {
                MoveRight(actionEvent->GetActionScale());
                break;
            }

            case Action::MOVE_LEFT:
            {
                MoveLeft(actionEvent->GetActionScale());
                break;
            }

            case Action::AIM: 
            {
                UpdateShootDirection(actionEvent->GetActionDirection(), actionEvent->IsPointActionDirection());
                break;
            }

            default:
                break;
        }
    }
}

void Player::OnCollision(BoxCollideable* other, CollisionDirection direction)
{
    sf::FloatRect otherCollider = other->GetBoundingBox();
    int32_t collisionDirection = static_cast<int32_t>(direction);
    
    if (typeid(*other) == typeid(class Enemy) 
        && (m_HealthState == HealthState::OK)
        && !m_IsSkullRolling)
    {
        Damage();
    }

    if (typeid(*other) == typeid(class CollideableTile))
    {
        if (collisionDirection & static_cast<int32_t>(CollisionDirection::BOTTOM))
        {
            if (collisionDirection & static_cast<int32_t>(CollisionDirection::IN_BOTTOM))
            {
                m_InGroundCollision = true;
                LOG_DEBUG("IN BOTTOM COLLISION");
            }
            
            if (!m_InCeilingCollision) 
            {
                m_Velocity.y = GRAVITY;
                m_Position.y = otherCollider.top - (m_BoundingBox.height / 2);
                m_IsGrounded = true;
                m_JumpCount = 1;
                //LOG_DEBUG("BOTTOM COLLISION");
            }
        }
        else if (collisionDirection & static_cast<int32_t>(CollisionDirection::TOP))
        {
            if (collisionDirection & static_cast<int32_t>(CollisionDirection::IN_TOP))
            {
                m_InCeilingCollision = true;
                LOG_DEBUG("IN TOP COLLISION");
            }

            if (!m_InGroundCollision) 
            {
                m_Velocity.y = 0.f;
                m_Position.y = otherCollider.top + otherCollider.height + (m_BoundingBox.height / 2);
                //LOG_DEBUG("TOP COLLISION");
            }
        }
        else if (collisionDirection & static_cast<int32_t>(CollisionDirection::LEFT))
        {
            m_Velocity.x = 0.f;
            m_Position.x = otherCollider.left + otherCollider.width + (m_BoundingBox.width / 2);
            //LOG_DEBUG("LEFT COLLISION");
        }
        else if (collisionDirection & static_cast<int32_t>(CollisionDirection::RIGHT))
        {
            m_Velocity.x = 0.f;
            m_Position.x = otherCollider.left - (m_BoundingBox.width / 2);
            //LOG_DEBUG("RIGHT COLLISION");
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

    // Apply gravity
    m_Velocity.y += GRAVITY;
    
    // If player isn't moving, he starts to slow down
    if (!m_InputManager->HasAction(Action::MOVE_RIGHT)
        && !m_InputManager->HasAction(Action::MOVE_LEFT)) 
    {
        m_Velocity.x *= SLOWDOWN_RATE;
    }

    // Update climbing status if necessary
    if (m_IsClimbing && !m_CanClimb)
    {
        m_IsClimbing = false;
    }
    // Reset the vertical velocity if climbing but not moving
    else if (!m_InputManager->HasAction(Action::MOVE_UP)
        && !m_InputManager->HasAction(Action::MOVE_DOWN) 
        && m_IsClimbing)
    { 
        m_Velocity.y = 0.f;
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

    UIViewModel::GetInstance()->SetPlayerPosition(m_Position);
}

void Player::MoveUp()
{
    // Climb up
    if (m_CanClimb && !m_IsSkullRolling)
    {
        m_Velocity.y = -CLIMB_SPEED;
        m_IsClimbing = true;
        m_JumpCount = 0;
    }
    else if (!m_CanClimb && m_IsClimbing)
    {
        // Player gets a little force if he's jumping when getting out of the ladder
        m_Velocity.y = -(JUMP_FORCE * 0.75f);
    }
    // Jump
    else if (m_JumpCount != 0)
    {
        m_JumpCount -= 1;
        m_Velocity.y = -JUMP_FORCE;
    }
    
    m_IsGrounded = false;
}

void Player::MoveDown()
{
    if (m_IsClimbing)
    {
        m_Velocity.y = CLIMB_SPEED;

        if (m_IsGrounded)
        {
            m_IsClimbing = false;
        }
    }
}

void Player::MoveRight(const float scale)
{
    m_Velocity.x = fmin(m_Velocity.x + MOVE_SPEED_INC, scale * MOVE_SPEED_MAX);
}

void Player::MoveLeft(const float scale)
{
    m_Velocity.x = fmax(m_Velocity.x - MOVE_SPEED_INC, scale * -MOVE_SPEED_MAX);
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

void Player::UpdateShootDirection(const sf::Vector2f& direction, const bool isPoint) 
{
    sf::Vector2f shootDirection = direction;

    if (isPoint) 
    {
        shootDirection = direction - m_Position;
    }

    // Normalize the vector
    // TODO : Make a normalize function in a MathUtils file
    float magnitude = std::sqrt(shootDirection.x * shootDirection.x + shootDirection.y * shootDirection.y);
    shootDirection = shootDirection / magnitude;

    m_ShootDirection = shootDirection;
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
        m_Bullets.emplace_back(m_TextureManager, m_ShootDirection, m_Position);

        m_AmmunitionsNumber--;
        UIViewModel::GetInstance()->SetAmmunitionsNumber(m_AmmunitionsNumber);
        
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