#pragma once

namespace SeekASoul 
{
	namespace Engine 
	{
		class Binding
		{
		public:
			virtual bool operator==(Binding* other) = 0;
		};
	}
}