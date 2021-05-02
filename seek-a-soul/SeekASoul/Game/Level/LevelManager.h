#pragma once

#include <Game/Map/GameMap.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		class LevelManager : public sf::Drawable
		{
		public:
			LevelManager(const std::shared_ptr<Engine::InputManager>& inputManager, const std::shared_ptr<Engine::TextureManager>& textureManager);
			~LevelManager();

			void Update(float deltaTime);
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
			void RenderDebugMenu(sf::RenderTarget& target);
	
			inline const GameMap& GetGameMap() const { return m_GameMap; };
			inline const sf::Vector2u GetLevelBounds() const { return { m_LevelWidth * GameMap::TILE_SIZE.x, m_LevelHeight * GameMap::TILE_SIZE.y }; };
			inline const Player& GetPlayerOnMap() const { return m_GameMap.GetPlayer(); };

		private:
			void ChooseLevel();
			void LoadLevel(bool restart);
			void ManageLevelChange();

			void OnEvent(const Engine::Event* evnt);

			//====================//

			std::shared_ptr<Engine::TextureManager> m_TextureManager;
			GameMap m_GameMap;

			enum class LevelState 
			{
				PENDING		= 0,
				SELECTING	= 1,
				LOADING		= 2,
				PLAYING		= 3,
				OVER		= 4
			} m_CurrentState;

			int m_LevelChoice;
			unsigned int m_CurrentLevel;
			unsigned int m_LevelWidth;
			unsigned int m_LevelHeight;
		};
	}
}