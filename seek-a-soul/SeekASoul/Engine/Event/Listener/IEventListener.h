#pragma once

class IEventListener 
{
public: 
	virtual bool operator==(IEventListener* other) const = 0;
	virtual void Fire(Event* evnt) const = 0;
};