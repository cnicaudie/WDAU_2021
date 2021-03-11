#pragma once

class IEventHandler 
{
public: 
	virtual bool operator==(IEventHandler* other) const = 0;
	virtual void Fire() const = 0;
};