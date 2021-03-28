#pragma once

class Binding
{
public:
	virtual bool operator==(Binding* other) = 0;
};