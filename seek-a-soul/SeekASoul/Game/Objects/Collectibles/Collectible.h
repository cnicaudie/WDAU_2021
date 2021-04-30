#pragma once

namespace SeekASoul
{
	namespace Gameplay
	{
		class Collectible : public Engine::BoxCollideable
		{
		public:
			Collectible();
			virtual ~Collectible();

			void OnTrigger(Engine::BoxCollideable* other) override;

			inline const bool WasCollected() const { return m_WasCollected; };

		protected:
			bool m_WasCollected;
		};
	}
}