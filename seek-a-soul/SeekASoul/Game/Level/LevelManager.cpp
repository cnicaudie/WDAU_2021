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
	LoadLevel(false);

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
	if (!GameManager::GetInstance()->HasGameStarted())
	{
		if (m_CurrentState == LevelState::SELECTING) 
		{
			ChooseLevel();
		}
	} 
	else
	{
		m_Map.Update(deltaTime);

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
		else if (m_CurrentState == LevelState::LOADING)
		{
			LOG_INFO("Reloading current level...");
			GameManager::GetInstance()->Restart();
			LoadLevel(true);
			LOG_INFO("Done!");
		}
	}
}

void LevelManager::OnEvent(const Event* evnt)
{
	if (const LevelEvent* actionEvent = dynamic_cast<const LevelEvent*>(evnt))
	{
		switch(actionEvent->GetLevelStatus()) 
		{
			case LevelStatus::SELECT:
			{
				m_CurrentState = LevelState::SELECTING;
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
				LOG_DEBUG("Restart level");
				m_CurrentState = LevelState::LOADING;
			}

			default:
				break;
		}
	} 
	else if (evnt->GetEventType() == EventType::START_GAME) 
	{
		LoadLevel(false);
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
	m_Map.LoadTileMap(levelData.first, sf::Vector2u(m_LevelWidth, m_LevelHeight));

	// Initialize other elements
	m_Map.InitObjectsAndEntities(configKeymap, restart);

	m_CurrentState = LevelState::PLAYING;
}

void LevelManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Map);
}

void LevelManager::RenderDebugMenu(sf::RenderTarget& target)
{
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	ImGui::SliderInt("Level", &m_CurrentLevel, 0, MAX_LEVEL);
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	if (ImGui::Button("Restart"))
	{
		GameManager::GetInstance()->Restart();
		LoadLevel(true);
	}

	m_Map.RenderDebugMenu(target);
};

void LevelManager::ChooseLevel()
{
	ImGui::SetNextWindowSize(ImVec2(270.f, 60.f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(600.f, 400.f), ImGuiCond_FirstUseEver);
	ImGui::Begin("Select a level !");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	ImGui::SliderInt("Level", &m_CurrentLevel, 0, MAX_LEVEL);
	ImGui::SameLine();

	if (ImGui::Button("Confirm"))
	{
		m_CurrentState = LevelState::LOADING;
	}

	ImGui::End();
}