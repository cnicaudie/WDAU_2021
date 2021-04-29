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

			void UpdateStrategies();

			void Register(AIEntity* ai);
			void Unregister(AIEntity* ai);

		private:
			AIManager();
			~AIManager();

			//====================//

			static AIManager* m_AIManager; // Singleton instance

			std::vector<AIEntity*> m_AIs;
			//std::vector<AIStrategy*> m_Strategies;
		};
	}
}