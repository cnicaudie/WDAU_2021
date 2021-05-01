#pragma once

#include <AI/Pathfinding/Node.h>

namespace SeekASoul
{
	namespace AI
	{
		class Graph
		{
		public:
			Graph(const std::vector<Node>& nodes);

			const bool IsInGraph(const Node& node) const;

		private:
			std::vector<Node> m_Nodes;
		};
	}
}