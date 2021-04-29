#pragma once

namespace SeekASoul 
{
	namespace Engine 
	{
		class IAction 
		{
		public:
			virtual const bool operator==(IAction* other) const = 0;
			virtual const bool IsAimX() const = 0;
			virtual const bool IsAimY() const = 0;
		};
	}
}