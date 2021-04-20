#pragma once

#include <Game/Action.h>
#include <Engine/Event/EventTypes/ActionEvent.h>

class Binding;

class InputManager 
{
public:
	InputManager();
	~InputManager();

	void Update();
	inline void RenderDebugMenu(sf::RenderTarget& target) 
	{
		if (ImGui::CollapsingHeader("Mouse position"))
		{
			ImGui::Text("Game :");
			ImGui::SameLine();
			ImGui::Text("X: %f", m_GameMousePosition.x);
			ImGui::SameLine();
			ImGui::Text("Y: %f", m_GameMousePosition.y);
			ImGui::Text("GUI :");
			ImGui::SameLine(); 
			ImGui::Text("X: %f", m_GUIMousePosition.x);
			ImGui::SameLine();
			ImGui::Text("Y: %f", m_GUIMousePosition.y);
		}
	};

	void ManageInputEvents(const sf::Event& event);

	void AddAction(Binding* key);
	void RemoveAction(Binding* key);

	inline bool HasAction(Action action) const
	{
		auto it = std::find_if(m_CurrentActions.begin(), m_CurrentActions.end(), [&](const std::shared_ptr<ActionEvent> actionEvent)
			{
				return actionEvent->GetActionType() == action;
			});
		
		return it != m_CurrentActions.end();
	}

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
	std::map<Binding*, Action> m_ActionBinding;

	sf::Vector2f m_GameMousePosition;
	sf::Vector2f m_GUIMousePosition;
	sf::Vector2f m_AimJoystickPosition;
};