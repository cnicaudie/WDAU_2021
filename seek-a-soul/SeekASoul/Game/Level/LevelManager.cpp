#include <stdafx.h>
#include "LevelManager.h"
#include <Engine/Resources/FileReader.h>
#include <Engine/Event/EventTypes/LevelEvent.h>

static constexpr unsigned int MAX_LEVEL = 1;

LevelManager::LevelManager(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager)
	: m_TextureManager{ textureManager }
	, m_GameMap { inputManager, textureManager }
	, m_CurrentState(LevelState::PENDING)
	, m_CurrentLevel(0)
{
	// Configure EventListeners
	EventListener<LevelManager, Event> listenerEvent(this, &LevelManager::OnEvent);
	EventListener<LevelManager, LevelEvent> listenerLevelEvent(this, &LevelManager::OnEvent);
	EventManager::GetInstance()->AddListener(listenerEvent);
	EventManager::GetInstance()->AddListener(listenerLevelEvent);
}

LevelManager::~LevelManager()
{
	EventListener<LevelManager, Event> listenerEvent(this, &LevelManager::OnEvent);
	EventListener<LevelManager, LevelEvent> listenerLevelEvent(this, &LevelManager::OnEvent);
	EventManager::GetInstance()->RemoveListener(listenerEvent);
	EventManager::GetInstance()->RemoveListener(listenerLevelEvent);
}

void LevelManager::Update(float deltaTime) 
{
	if (!GameManager::GetInstance()->HasGameStarted())
	{
		if (m_CurrentState == LevelState::SELECTING) 
		{
			ChooseLevel();
		}
	} 
	else
	{
		m_GameMap.Update(deltaTime);
		
		ManageLevelChange();
	}

}

void LevelManager::ManageLevelChange()
{
	if (!GameManager::GetInstance()->IsGameOver())
	{
		if (m_CurrentState == LevelState::OVER
			|| (m_CurrentState == LevelState::LOADING && m_CurrentLevel > MAX_LEVEL))
		{
			std::shared_ptr<Event> eventType = std::make_shared<Event>(EventType::END_GAME);
			EventManager::GetInstance()->Fire(eventType);
		}
		else if (m_CurrentState == LevelState::LOADING)
		{
			LOG_INFO("Loading next level...");
			LoadLevel(false);
			LOG_INFO("Done!");
		}
	}
}

void LevelManager::OnEvent(const Event* evnt)
{
	if (evnt->GetEventType() == EventType::START_GAME)
	{
		LOG_INFO("Loading start level...");
		LoadLevel(false);
		LOG_INFO("Done!");
	}
	else if (const LevelEvent* actionEvent = dynamic_cast<const LevelEvent*>(evnt))
	{
		switch(actionEvent->GetLevelStatus()) 
		{
			case LevelStatus::SELECT:
			{
				if (m_CurrentState == LevelState::SELECTING) 
				{
					m_CurrentState = LevelState::PENDING;
				} 
				else 
				{
					LOG_INFO("Selecting start level...");
					m_CurrentState = LevelState::SELECTING;
				}
				break;
			}

			case LevelStatus::FAILURE:
			{
				m_CurrentState = LevelState::OVER;
				break;
			}
			
			case LevelStatus::SUCCESS:
			{
				LOG_INFO("Successfully ended level " << m_CurrentLevel << " !");
				m_CurrentLevel += 1;
				m_CurrentState = LevelState::LOADING;
				break;
			}

			case LevelStatus::RESTART: 
			{
				LOG_INFO("Reloading current level...");
				m_CurrentState = LevelState::LOADING;
				LoadLevel(true);
				LOG_INFO("Done!");
			}

			default:
				break;
		}
	}
}

void LevelManager::LoadLevel(bool restart)
{
	std::string levelFileName(".\\Assets\\Levels\\level" + std::to_string(m_CurrentLevel) + ".txt");
	std::string levelConfigFileName(".\\Assets\\Levels\\level" + std::to_string(m_CurrentLevel) + "_config.txt");

	std::pair<std::vector<int>, sf::Vector2u> levelData = FileReader::ReadLevelFromFile(levelFileName);
	std::map<std::string, std::vector<std::string>> configKeymap = FileReader::ReadConfigFile(levelConfigFileName);

	m_LevelWidth = levelData.second.x;
	m_LevelHeight = levelData.second.y;

	// Load the tile map
	m_GameMap.LoadTileMap(levelData.first, sf::Vector2u(m_LevelWidth, m_LevelHeight));

	// Initialize other elements
	m_GameMap.InitObjectsAndEntities(configKeymap, restart);

	m_CurrentState = LevelState::PLAYING;
}

void LevelManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_GameMap);
}

void LevelManager::RenderDebugMenu(sf::RenderTarget& target)
{
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	ImGui::SliderInt("Level", &m_LevelChoice, 0, MAX_LEVEL);
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	
	if (ImGui::Button("Restart"))
	{
		m_CurrentLevel = m_LevelChoice;
		std::shared_ptr<LevelEvent> levelEvent = std::make_shared<LevelEvent>(LevelStatus::RESTART);
		EventManager::GetInstance()->Fire(levelEvent);
	}

	m_GameMap.RenderDebugMenu(target);
};

void LevelManager::ChooseLevel()
{
	const float menuWidth = 270.f;
	const float menuHeight = 60.f;
	const float menuPosX = 600.f;
	const float menuPosY = 400.f;

	ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(menuPosX, menuPosY), ImGuiCond_FirstUseEver);
	ImGui::Begin("Select a level !");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	ImGui::SliderInt("Level", &m_LevelChoice, 0, MAX_LEVEL);
	ImGui::SameLine();

	if (ImGui::Button("Confirm"))
	{
		m_CurrentState = LevelState::LOADING;
		m_CurrentLevel = m_LevelChoice;
	}

	ImGui::End();
}