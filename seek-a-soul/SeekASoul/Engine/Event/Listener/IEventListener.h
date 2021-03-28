#pragma once

class IEventListener 
{
public: 
	virtual bool operator==(IEventListener* other) const = 0;
	virtual void Fire() const = 0;
};