#include <stdafx.h>
#include "AIPatrol.h"

namespace SeekASoul
{
	namespace AI
	{
		AIPatrol::AIPatrol() 
		{
			// TODO
		}

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
		}
	}
}