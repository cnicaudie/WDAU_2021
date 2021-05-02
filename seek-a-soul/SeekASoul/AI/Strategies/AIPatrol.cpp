#include <stdafx.h>
#include "AIPatrol.h"

namespace SeekASoul
{
	namespace AI
	{
		AIPatrol::AIPatrol() 
		{}

		void AIPatrol::ExecuteStrategy(AIEntity* ai) const
		{
			if (ai->CanDoAction(AIActionType::MOVE_RIGHT)) 
			{
				ai->MoveRight();
			}
			else if (ai->CanDoAction(AIActionType::MOVE_LEFT))
			{
				ai->MoveLeft();
			}
			else 
			{
				ai->Stop();
			}
		}
	}
}