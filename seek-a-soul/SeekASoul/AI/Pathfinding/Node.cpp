#include <stdafx.h>
#include "Node.h"

namespace SeekASoul
{
	namespace AI
	{
		Node::Node(const sf::Vector2f& position)
			: m_GCost(INT_MAX)
			, m_HCost(INT_MAX)
			, m_FCost(INT_MAX)
			, m_Position(position)
		{
			static int counter = 0;
			m_ID = counter++;
		}

		const std::vector<Node> Node::GetRelatedNodes(const std::vector<AIActionType> possibleActions) const 
		{
		}
	}
}