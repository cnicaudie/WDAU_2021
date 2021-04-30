#include <stdafx.h>
#include "Door.h"
#include <Game/GameplayIncludes.h>
#include <Game/Entities/Player.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		Door::Door(const sf::Vector2f& centerPosition, const sf::Vector2f& size)
			: m_rColor{ 1.f }
			, m_gColor{ 0.3f }
			, m_bColor{ 0.2f }
			, m_IsDoorOpen(false)
			, m_IsPlayingEndGame(false)
		{
			m_DoorRect.setSize(size);
			m_DoorRect.setOrigin(size * 0.5f);
			m_DoorRect.setPosition(centerPosition);

			m_DoorRect.setFillColor(sf::Color::Transparent);
			m_DoorRect.setOutlineThickness(5);
			m_DoorRect.setOutlineColor(sf::Color{ static_cast<uint8_t>(m_rColor * 255.0f), static_cast<uint8_t>(m_gColor * 255.0f), static_cast<uint8_t>(m_bColor * 255.0f) });

			SetTrigger(true);
			SetBoundingBox(centerPosition, size);
		}

		Door::~Door()
		{
			LOG_INFO("Destroyed Door");
		}

		void Door::Update(float deltaTime)
		{
			if (m_IsDoorOpen || m_IsPlayingEndGame)
			{
				// Multicolor door
				m_rColor = fmodf(m_rColor + deltaTime, 1.f);
				m_gColor = fmodf(m_gColor + deltaTime, 1.f);
				m_bColor = fmodf(m_bColor + deltaTime, 1.f);
				m_DoorRect.setOutlineColor(sf::Color{ static_cast<uint8_t>(m_rColor * 255.0f), static_cast<uint8_t>(m_gColor * 255.0f), static_cast<uint8_t>(m_bColor * 255.0f) });
			}
		}

		void Door::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_DoorRect);
		}

		void Door::OnTrigger(Engine::BoxCollideable* other)
		{
			Player* player = dynamic_cast<Player*>(other);

			if (player != nullptr && m_IsDoorOpen && !m_IsPlayingEndGame)
			{	
				StartEndGame();
				std::shared_ptr<LevelEvent> eventType = std::make_shared<LevelEvent>(LevelStatus::SUCCESS);
				Engine::EventManager::GetInstance()->Fire(eventType);
			}
		}

		void Door::StartEndGame()
		{
			LOG_INFO("Someone walked through door !");
			m_IsPlayingEndGame = true;
		}
	}
}