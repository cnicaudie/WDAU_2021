#pragma once

namespace SeekASoul
{
	namespace AI
	{
		class AIEntity;

		class AIManager 
		{
		public:
			static AIManager* GetInstance();
			AIManager(const AIManager& aiManager) = delete;
			void operator=(const AIManager& aiManager) = delete;

			void Update();

			void Register(AIEntity* ai);
			void Unregister(AIEntity* ai);

		private:
			AIManager();
			~AIManager();

			void UpdateStrategies();

			//====================//

			static AIManager* m_AIManager; // Singleton instance

			std::vector<AIEntity*> m_AIs;
			//std::vector<AIStrategy*> m_Strategies;
		};
	}
}