#pragma once

#include <set>
#include <Game/Action.h>
#include <Engine/Input/Bindings/Binding.h>
#include <Engine/Event/EventTypes/ActionEvent.h>

class InputManager 
{
public:
	InputManager(const sf::RenderWindow* window);
	~InputManager();

	void Update();
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

	void UpdateMousePosition(const sf::RenderWindow& gameWindow);
	inline const sf::Vector2f GetMousePosition() const { return m_MousePosition; };

private:
	const sf::RenderWindow* m_Window;

	std::vector<std::shared_ptr<ActionEvent>> m_CurrentActions;
	std::map<Binding*, Action> m_ActionBinding;

	sf::Vector2f m_MousePosition;
	sf::Vector2f m_AimJoystickPosition;
};