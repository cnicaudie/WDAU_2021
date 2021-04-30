#pragma once

namespace SeekASoul
{
	namespace Engine
	{
		class ActionEvent;
		class Binding;
		class IAction;

		class InputManager 
		{
		public:
			InputManager();
			~InputManager();

			void AddActionBinding(std::shared_ptr<Binding> binding, std::shared_ptr<IAction> action);

			void Update();
			void RenderDebugMenu(sf::RenderTarget& target);
			void ManageInputEvents(const sf::Event& event);

			void AddAction(std::shared_ptr<Binding> binding);
			void RemoveAction(std::shared_ptr<Binding> binding);
			bool HasAction(IAction* action) const;

			inline void UpdateMousePosition(const sf::RenderWindow& gameWindow, bool isInGame)
			{
				const sf::Vector2f& mousePosition = gameWindow.mapPixelToCoords(sf::Mouse::getPosition(gameWindow));

				if (isInGame) 
				{
					m_GameMousePosition = mousePosition;
				} 
				else 
				{
					m_GUIMousePosition = mousePosition;
				}
			};

		private:
			std::vector<std::shared_ptr<ActionEvent>> m_CurrentActions;
			std::map<std::shared_ptr<Binding>, std::shared_ptr<IAction>> m_ActionBinding;

			sf::Vector2f m_GameMousePosition;
			sf::Vector2f m_GUIMousePosition;
			sf::Vector2f m_AimJoystickPosition;
		};
	}
}