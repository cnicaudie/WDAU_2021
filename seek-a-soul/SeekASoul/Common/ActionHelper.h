#pragma once

#include <iostream>
#include <Engine/Log/Log.h>
#include <AI/AIActionType.h>
#include <Game/Actions/ActionType.h>

namespace SeekASoul 
{
	namespace Common
	{
		static const Gameplay::ActionType ToGameAction(const AI::AIActionType aiAction) 
		{
			switch (aiAction)
			{
				case AI::AIActionType::NONE:
				{
					return Gameplay::ActionType::NONE;
				}

				case AI::AIActionType::MOVE_UP:
				{
					return Gameplay::ActionType::MOVE_UP;
				}
			
				case AI::AIActionType::MOVE_DOWN:
				{
					return Gameplay::ActionType::MOVE_DOWN;
				}

				case AI::AIActionType::MOVE_LEFT:
				{
					return Gameplay::ActionType::MOVE_LEFT;
				}

				case AI::AIActionType::MOVE_RIGHT:
				{
					return Gameplay::ActionType::MOVE_RIGHT;
				}

				default:
					LOG_WARNING("The AI action " << static_cast<int>(aiAction) << " does not match with any Gameplay action.");
					return Gameplay::ActionType::NONE;
			}
		}
	}
}