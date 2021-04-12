#include <stdafx.h>
#include "LevelManager.h"
#include <Engine/Resources/FileReader.h>
#include <Engine/Event/EventTypes/LevelEvent.h>

static constexpr unsigned int MAX_LEVEL = 1;

LevelManager::LevelManager(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager)
	: m_TextureManager{ textureManager }
	, m_Map { inputManager, textureManager }
	, m_CurrentState(LevelState::LOADING)
	, m_CurrentLevel(0)
{
	LoadLevel();

	EventListener<LevelManager, LevelEvent> listener(this, &LevelManager::OnEvent);
	EventManager::GetInstance()->AddListener(listener);
}

LevelManager::~LevelManager()
{
	EventListener<LevelManager, LevelEvent> listener(this, &LevelManager::OnEvent);
	EventManager::GetInstance()->RemoveListener(listener);
}

void LevelManager::Update(float deltaTime) 
{
	m_Map.Update(deltaTime);

	if (!GameManager::GetInstance()->IsGameOver() && m_CurrentState == LevelState::LOADING)
	{
		if (m_CurrentLevel > MAX_LEVEL)
		{
			std::shared_ptr<Event> eventType = std::make_shared<Event>(EventType::END_GAME);
			EventManager::GetInstance()->Fire(eventType);
		} 
		else 
		{
			LOG_INFO("Load next level...");
			LoadLevel();
		}
	}
}

void LevelManager::OnEvent(const Event* evnt)
{
	if (const LevelEvent* actionEvent = dynamic_cast<const LevelEvent*>(evnt))
	{
		switch(actionEvent->GetEndLevelType()) 
		{
			case EndLevelType::SUCCESS:
			{
				LOG_INFO("Successfully ended level " << m_CurrentLevel << " !");
				m_CurrentLevel += 1;
				m_CurrentState = LevelState::LOADING;
				break;
			}

			case EndLevelType::FAILURE:
			{
				// Reload level ? Display UI ?
				break;
			}

			default:
				break;
		}
	}
}

void LevelManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Map);
}

void LevelManager::LoadLevel()
{
	std::string levelFileName(".\\Assets\\Levels\\level" + std::to_string(m_CurrentLevel) + ".txt");
	std::string levelConfigFileName(".\\Assets\\Levels\\level" + std::to_string(m_CurrentLevel) + "_config.txt");
	
	std::pair<std::vector<int>, sf::Vector2u> levelData = FileReader::ReadLevelFromFile(levelFileName);
	std::map<std::string, std::vector<std::string>> configKeymap = FileReader::ReadConfigFile(levelConfigFileName);
	
	m_LevelWidth = levelData.second.x;
	m_LevelHeight = levelData.second.y;

	// Load the tile map
	m_Map.LoadTileMap(levelData.first, sf::Vector2u(m_LevelWidth, m_LevelHeight));

	// Initialize other elements
	m_Map.InitObjectsAndEntities(configKeymap);

	m_CurrentState = LevelState::PLAYING;
}