#pragma once

#include <AI/Pathfinding/Graph.h>

namespace SeekASoul
{
	namespace AI
	{
		class Pathfinder // Singleton
		{
		public:
			Pathfinder();

			void InitGraphs(const std::vector<Graph> graphs);

			// Code A* solver directly here (will change later if using other Pathfinding methods)
		private:
			// Graphs
			std::vector<Graph> m_Graphs;
		};
	}
}