#include <stdafx.h>
#include "Player.h"
#include "Enemy.h"
#include <Engine/Event/EventTypes/ActionEvent.h>
#include <Engine/Maths/Maths.h>
#include <Engine/Time/Time.h>
#include <Game/GameplayIncludes.h>
#include <Game/Map/Tiles/CollideableTile.h>
#include <Game/Map/Tiles/ClimbableTile.h>
#include <Game/Map/Tiles/DeadlyTile.h>
#include <Game/Objects/Collectibles/SoulChunk.h>
#include <Game/Objects/MovingPlatform.h>
#include <AI/Threat/ThreatLevel.h>
#include <AI/Threat/ThreatTeam.h>
#include <AI/Threat/ThreatManager.h>

namespace SeekASoul
{
    namespace Gameplay
    {
        static const sf::Vector2i PLAYER_SPRITE_SIZE{ 32, 56 };

        static constexpr uint64_t THROW_COOLDOWN = 500;
        static constexpr uint64_t DAMAGE_COOLDOWN = 1000;
        static constexpr uint64_t SKULL_ROLL_COOLDOWN = 5000;

        static constexpr unsigned int MAX_HEALTH_POINTS = 200;
        static constexpr unsigned int MAX_AMMUNITIONS = 20;

        static constexpr float MAX_BONE_RANGE = 400.f;

        static constexpr float MOVE_SPEED_MAX = 200.0f;
        static constexpr float MOVE_SPEED_INC = 10.0f;
        static constexpr float CLIMB_SPEED = 100.0f;
        static constexpr float JUMP_FORCE = 400.0f;
        static constexpr float COLLISION_FORCE = 250.0f;
        static constexpr float SLOWDOWN_RATE = 0.9f;
        static constexpr float GRAVITY = 9.8f;

        Player::Player(const std::shared_ptr<Engine::InputManager>& inputManager, const std::shared_ptr<Engine::TextureManager>& textureManager)
            : Entity(textureManager, { 50.f, 50.f }, MAX_HEALTH_POINTS)
            , Animated(PLAYER_SPRITE_SIZE, textureManager->GetTextureFromName("PLAYER_SHEET"))
            , AI::Threat(AI::ThreatLevel::CALM, AI::ThreatTeam::A)
            , m_InputManager{ inputManager }
            , m_CurrentState(PlayerState::IDLE)
            , m_JumpCount(1)
            , m_SoulChunksCollected(0)
            , m_IsClimbing(false)
            , m_CanClimb(false)
            , m_IsSkullRolling(false)
            , m_LastSkullRollTime(0)
            , m_LastThrowTime(0)
            , m_ThrowDirection{ 0.f, 0.f }
            , m_Bones{}
            , m_InfiniteAmmos(false)
            , m_InGroundCollision(false)
            , m_InCeilingCollision(false)
            , m_IsOnMovingPlatform(false)
            , m_Platform(nullptr)
        {
            SetBoundingBox(m_Position, static_cast<sf::Vector2f>(PLAYER_SPRITE_SIZE));
    
            // Configure an EventListener for action events
            Engine::EventListener<Player, Engine::ActionEvent> listener(this, &Player::OnEvent);
            Engine::EventManager::GetInstance()->AddListener(listener);

            // Register threat
            AI::ThreatManager::GetInstance()->RegisterThreat(this, &m_Position);
        }
        
        Player::~Player() 
        {
            Engine::EventListener<Player, Engine::ActionEvent> listener(this, &Player::OnEvent);
            Engine::EventManager::GetInstance()->RemoveListener(listener);

            AI::ThreatManager::GetInstance()->UnregisterThreat(this);
        }

        void Player::Reset(const sf::Vector2f& position, bool restart)
        {
            m_StartPosition = position;
            SetPosition(position);
    
            m_AmmunitionsNumber = MAX_AMMUNITIONS;
    
            if (restart) 
            {
                m_AnimationSprite.setColor(sf::Color::White);
                m_HealthPoints = MAX_HEALTH_POINTS;
                m_HealthState = HealthState::OK;
                m_CurrentState = PlayerState::IDLE;
                m_JumpCount = 1;
                m_IsClimbing = false;
                m_CanClimb = false;
                m_IsSkullRolling = false;
                m_IsOnMovingPlatform = false;
                m_Platform = nullptr;

                // Loose 2 soul chuncks per restart
                m_SoulChunksCollected >= 2 ? m_SoulChunksCollected -= 2 : m_SoulChunksCollected = 0;

                m_LastSkullRollTime = 0;
                m_LastThrowTime = 0;
            }
    
            // Update UIViewModel values
            UI::UIViewModel::GetInstance()->SetHealthPoints(m_HealthPoints);
            UI::UIViewModel::GetInstance()->SetMaxHealthPoints(MAX_HEALTH_POINTS);
            UI::UIViewModel::GetInstance()->SetAmmunitionsNumber(m_AmmunitionsNumber);
            UI::UIViewModel::GetInstance()->SetSoulChunksNumber(m_SoulChunksCollected);
        }

        void Player::SetPosition(const sf::Vector2f& position)
        {
            m_Position = position;
            SetCenter(m_Position);
            SetAnimatedSpritePosition(m_Position);
        }

        void Player::Update(float deltaTime)
        {
            if (m_HealthState == HealthState::DEAD || GameManager::GetInstance()->IsGameOver())
            {
                return;
            }

            uint64_t now = Engine::Time::GetCurrentTimeAsMilliseconds();

            if (m_IsSkullRolling) 
            {
                UpdateSkullRollCooldown(now);
            }
    
            UpdateBoundingBox();

            Move(deltaTime);

            // If in ground and ceiling after collision check, player stays skull rolling
            if (m_InGroundCollision && m_InCeilingCollision)
            {
                m_IsSkullRolling = true;
                m_LastSkullRollTime = now;
            }

            if (m_HealthState == HealthState::DAMAGED)
            {
                UpdateVisualDamage(now);
            }

            // Update bullets and check for impacts
            ManageBones(deltaTime);

            // Update player's animation
            ComputeNextPlayerState();
            PlayAnimation(static_cast<int>(m_CurrentState));
        }

        void Player::OnEvent(const Engine::Event* evnt)
        {
            if (const Engine::ActionEvent* actionEvent = dynamic_cast<const Engine::ActionEvent*>(evnt))
            {
                if (actionEvent->IsAim()) 
                {
                    UpdateThrowDirection(actionEvent->GetActionDirection(), actionEvent->IsPointActionDirection());
                }
                else 
                {
                    const Action* action = dynamic_cast<Action*>(actionEvent->GetAction());
                    switch (action->GetActionType())
                    {
                        case ActionType::BONE_THROW:
                        {
                            ThrowBone();
                            break;
                        }

                        case ActionType::SKULL_ROLL:
                        {
                            SkullRoll();
                            break;
                        }

                        case ActionType::MOVE_UP:
                        {
                            MoveUp();
                            break;
                        }

                        case ActionType::MOVE_DOWN:
                        {
                            MoveDown();
                            break;
                        }

                        case ActionType::MOVE_RIGHT:
                        {
                            MoveRight(actionEvent->GetActionScale());
                            break;
                        }

                        case ActionType::MOVE_LEFT:
                        {
                            MoveLeft(actionEvent->GetActionScale());
                            break;
                        }

                        default:
                            break;
                    }
                }
            }
        }

        void Player::OnCollision(Engine::BoxCollideable* other, Engine::CollisionDirection direction)
        {
            sf::FloatRect otherCollider = other->GetBoundingBox();
            int32_t collisionDirection = static_cast<int32_t>(direction);

            MovingPlatform* platform = dynamic_cast<MovingPlatform*>(other);

            // Collision with platform (player can go through the platform but stay on top of it when he's on it)
            if (platform != nullptr && collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::BOTTOM)
                && !(collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::IN_TOP)))
            {
                Action moveUp(ActionType::MOVE_UP);
                if (!m_InputManager->HasAction(&moveUp))
                {
                    m_Velocity.y = 0.f;
                }
        
                m_Position.y = (otherCollider.top - (m_BoundingBox.height / 2));
                m_JumpCount = 1;
                m_Platform = platform;
                m_IsOnMovingPlatform = true;
            }
            // Reset platform info
            else
            {
                m_Platform = nullptr;
                m_IsOnMovingPlatform = false;
            }

            // Collision with enemy
            if (typeid(*other) == typeid(class Enemy) && (m_HealthState == HealthState::OK) && !m_IsSkullRolling)
            {
                if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::LEFT))
                {
                    m_Velocity.x = COLLISION_FORCE;
                }
                else if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::RIGHT))
                {
                    m_Velocity.x = -COLLISION_FORCE;
                }
        
                if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::BOTTOM))
                {
                    m_Velocity.y = -COLLISION_FORCE;
                }

                Damage();
            }

            // Collision with a deadly tile
            if (typeid(*other) == typeid(class DeadlyTile))
            {
                Die();
            }
            // Collision with a collideable tile
            else if (typeid(*other) == typeid(class CollideableTile))
            {
                ApplyCollisionCorrection(collisionDirection, otherCollider);
            }
        }

        void Player::OnTrigger(Engine::BoxCollideable* other)
        {
            Collectible* collectible = dynamic_cast<Collectible*>(other);

            // Trigger with a collectible (if not collected, collects it)
            if (collectible != nullptr && !collectible->WasCollected())
            {
                if (typeid(*collectible).name() == typeid(class SoulChunk).name()) 
                {
                    LOG_INFO("Player collected SoulChunk !");
                    m_SoulChunksCollected += 1;
                    UI::UIViewModel::GetInstance()->SetSoulChunksNumber(m_SoulChunksCollected);
                }
            }

            // Trigger with a climbable tile (if on a ladder, he can climb)
            if (typeid(*other).name() == typeid(class ClimbableTile).name() && !m_IsSkullRolling)
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

            for (const Bone& b : m_Bones)
            {
                target.draw(b);
            }
        }

        void Player::RenderDebugMenu(sf::RenderTarget& target)
        {
            if (ImGui::CollapsingHeader("Player Menu"))
            {
                ImGui::Text("ThreatLevel : %d", static_cast<int>(GetThreatLevel()));
                ImGui::Text("Jump Count : %d", m_JumpCount);
                ImGui::Text("Is on plaform ? : %d", m_IsOnMovingPlatform);
                ImGui::Checkbox("Infinite Ammos", &m_InfiniteAmmos);
                
                ImGui::Text("Pos :");
                ImGui::SameLine();
                ImGui::Text("X: %f", m_Position.x);
                ImGui::SameLine();
                ImGui::Text("Y: %f", m_Position.y);

                ImGui::Text("Vel :");
                ImGui::SameLine();
                ImGui::Text("X: %f", m_Velocity.x);
                ImGui::SameLine();
                ImGui::Text("Y: %f", m_Velocity.y);
            }
        }

        void Player::ComputeNextPlayerState()
        {
            const float MOVE_ANIMATION_THRESHOLD = 50.f;

            if (m_IsClimbing)
            {
                m_CurrentState = PlayerState::CLIMBING;
            }
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
            Action moveUp(ActionType::MOVE_UP);
            Action moveDown(ActionType::MOVE_DOWN);
            Action moveRight(ActionType::MOVE_RIGHT);
            Action moveLeft(ActionType::MOVE_LEFT);

            const sf::Vector2f platformOffset = m_Platform != nullptr ? m_Platform->GetPlatformOffset() : sf::Vector2f{ 0.f, 0.f };
            sf::Vector2f tempVelocity(0.f, 0.f);

            // Reset in ground/ceiling collision checks
            m_InGroundCollision = false;
            m_InCeilingCollision = false;

            // Apply gravity
            m_Velocity.y += GRAVITY;
    
            // If player isn't moving, he starts to slow down
            if (!m_InputManager->HasAction(&moveRight)
                && !m_InputManager->HasAction(&moveLeft))
            {
                m_Velocity.x *= SLOWDOWN_RATE;
            }

            // Update climbing status if necessary
            if (m_IsClimbing && !m_CanClimb)
            {
                m_IsClimbing = false;
            }
            // Reset the vertical velocity if climbing but not moving
            else if (!m_InputManager->HasAction(&moveUp)
                && !m_InputManager->HasAction(&moveDown)
                && m_IsClimbing)
            {
                m_Velocity.y = 0.f;
            }

            // Check movement on X axis
            tempVelocity.x = m_IsOnMovingPlatform ? platformOffset.x + m_Velocity.x : m_Velocity.x;
            if (!GameManager::GetInstance()->CheckCollisions(this, tempVelocity * deltaTime).first)
            {
                m_Position += tempVelocity * deltaTime;
            }
            else if (m_IsOnMovingPlatform)
            {
                // Apply platform offset anyway
                m_Position += sf::Vector2f(platformOffset.x, 0.f) * deltaTime;
            }

            // Check movement on Y axis
            tempVelocity.x = 0.0f;
            tempVelocity.y = m_IsOnMovingPlatform ? platformOffset.y + m_Velocity.y : m_Velocity.y;
            if (!GameManager::GetInstance()->CheckCollisions(this, tempVelocity * deltaTime).first)
            {
                m_Position += tempVelocity * deltaTime;
            }
            else if (m_IsOnMovingPlatform)
            {
                // Apply platform offset anyway
                m_Position += sf::Vector2f(0.f, platformOffset.y) * deltaTime;
            }

            // Check if player fell down out of the level's lowest bound
            sf::Vector2u levelBounds = GameManager::GetInstance()->GetLevelBounds();
            CheckFallDown(static_cast<float>(levelBounds.y));

            // Apply new position
            SetPosition(m_Position);

            //Update UI view model
            UI::UIViewModel::GetInstance()->SetPlayerPosition(m_Position);
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
    
            m_Platform = nullptr;
            m_IsOnMovingPlatform = false;
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

        void Player::CheckFallDown(float maxBoundY)
        {
            // Player respawn at start position and get damaged if he falls down (over the level's bounds)
            if (m_BoundingBox.top + m_BoundingBox.height > maxBoundY)
            {
                const int FALL_DOWN_DAMAGE = 50;
                Damage(FALL_DOWN_DAMAGE);
                m_Position = m_StartPosition;
            }
        }

        void Player::ApplyCollisionCorrection(const int32_t& collisionDirection, sf::FloatRect& otherCollider)
        {
            if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::BOTTOM))
            {
                if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::IN_BOTTOM))
                {
                    m_InGroundCollision = true;
                }

                if (!m_InCeilingCollision)
                {
                    m_Velocity.y = GRAVITY;
                    m_Position.y = otherCollider.top - (m_BoundingBox.height / 2);
                    m_IsGrounded = true;
                    m_JumpCount = 1;
                }
            }
            else if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::TOP))
            {
                if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::IN_TOP))
                {
                    m_InCeilingCollision = true;
                }

                if (!m_InGroundCollision)
                {
                    m_Velocity.y = GRAVITY;
                    m_Position.y = otherCollider.top + otherCollider.height + (m_BoundingBox.height / 2);
                }
            }
            else if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::LEFT))
            {
                m_Velocity.x = 0.f;
                m_Position.x = otherCollider.left + otherCollider.width + (m_BoundingBox.width / 2);
            }
            else if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::RIGHT))
            {
                m_Velocity.x = 0.f;
                m_Position.x = otherCollider.left - (m_BoundingBox.width / 2);
            }
        }

        void Player::Damage()
        {
            const int DEFAULT_DAMAGE = static_cast<int>(Engine::Maths::GetRandom(10.f, 15.f));
            Damage(DEFAULT_DAMAGE);
        }

        void Player::Damage(const int value)
        {
            LOG_INFO("Player was damaged !");

            m_AnimationSprite.setColor(sf::Color::Red);
            m_HealthState = HealthState::DAMAGED;
            m_HealthPoints -= value;

            if (m_HealthPoints <= 0)
            {
                m_HealthPoints = 0;
                Die();
            }

            m_LastDamageTime = Engine::Time::GetCurrentTimeAsMilliseconds();

            UI::UIViewModel::GetInstance()->SetHealthPoints(m_HealthPoints);
        }

        void Player::UpdateVisualDamage(uint64_t now)
        {
            if (Engine::Maths::GetDifference(now, m_LastDamageTime) >= DAMAGE_COOLDOWN)
            {
                m_AnimationSprite.setColor(sf::Color::White);
                m_HealthState = HealthState::OK;
            }
        }

        void Player::UpdateBoundingBox()
        {
            if (m_IsSkullRolling)
            {
                SetBoundingBox(m_Position, sf::Vector2f(static_cast<float>(PLAYER_SPRITE_SIZE.x), PLAYER_SPRITE_SIZE.y * 0.5f));
            }
            else
            {
                SetBoundingBox(m_Position, static_cast<sf::Vector2f>(PLAYER_SPRITE_SIZE));
            }
        }

        void Player::UpdateThrowDirection(const sf::Vector2f& direction, const bool isPoint) 
        {
            sf::Vector2f shootDirection = direction;

            if (isPoint) 
            {
                shootDirection = direction - m_Position;
            }

            Engine::Maths::NormalizeVector(shootDirection);

            m_ThrowDirection = shootDirection;
        }

        void Player::ThrowBone()
        {
            uint64_t now = Engine::Time::GetCurrentTimeAsMilliseconds();

            if (Engine::Maths::GetDifference(now, m_LastThrowTime) >= THROW_COOLDOWN
                && (m_InfiniteAmmos || m_AmmunitionsNumber > 0)
                && !m_IsSkullRolling) 
            {
                m_Bones.emplace_back(m_TextureManager, m_ThrowDirection, m_Position);
        
                if (!m_InfiniteAmmos) 
                {
                    m_AmmunitionsNumber--;
                    UI::UIViewModel::GetInstance()->SetAmmunitionsNumber(m_AmmunitionsNumber);
                }

                m_LastThrowTime = now;
            }
        }

        void Player::SkullRoll()
        {
            uint64_t now = Engine::Time::GetCurrentTimeAsMilliseconds();
    
            if (m_IsSkullRolling 
                && Engine::Maths::GetDifference(now, m_LastSkullRollTime) >= (SKULL_ROLL_COOLDOWN * 0.1f)) // avoid immediate skull roll out
            {
                m_IsSkullRolling = false;
                m_LastSkullRollTime = now;
            }
            else if (Engine::Maths::GetDifference(now, m_LastSkullRollTime) >= SKULL_ROLL_COOLDOWN)
            {
                m_IsSkullRolling = true;
                m_LastSkullRollTime = now;
            }
        }

        void Player::UpdateSkullRollCooldown(uint64_t now)
        {
            if (Engine::Maths::GetDifference(now, m_LastSkullRollTime) >= SKULL_ROLL_COOLDOWN)
            {
                m_IsSkullRolling = false;
                m_LastSkullRollTime = now;
            }
        }

        void Player::ManageBones(float deltaTime)
        {
            // Update the bullets
            for (Bone& b : m_Bones)
            {
                b.Update(deltaTime);
            }

            // Check for impact
            int boneIndex = 0;
            for (Bone& b : m_Bones)
            {
                if (b.GetDistanceTraveled() > MAX_BONE_RANGE || b.HadImpact()) 
                {
                    m_Bones.erase(m_Bones.begin() + boneIndex);
                }

                boneIndex++;
            }
        }

        void Player::Die() 
        {
            LOG_INFO("Player died !");
            m_HealthState = HealthState::DEAD;
            m_AnimationSprite.setColor(sf::Color::Red);

            std::shared_ptr<LevelEvent> eventType = std::make_shared<LevelEvent>(LevelStatus::FAILURE);
            Engine::EventManager::GetInstance()->Fire(eventType);
        }
    }
}