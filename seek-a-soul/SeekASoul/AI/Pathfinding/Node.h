#pragma once

#include "Edge.h"

namespace SeekASoul
{
	namespace AI
	{
		class Node
		{
		public:
			Node(const sf::Vector2f& position);

			void SetParent(Node* parent) { m_Parent = parent; };
			const std::vector<Node> GetRelatedNodes(const std::vector<Edge> possibleActions) const;

			const bool operator==(const Node& other) const { return m_ID == other.m_ID; };

		private:
			int m_ID;
			int m_GCost;
			int m_HCost;
			int m_FCost;
			
			sf::Vector2f m_Position;
			Node* m_Parent;
			std::map<Edge, Node> m_RelatedNodes;
		};
	}
}