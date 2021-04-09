#pragma once

class Collectible : public BoxCollideable 
{
public:
	Collectible();
	virtual ~Collectible();

	void OnTrigger(BoxCollideable* other) override;

	inline const bool WasCollected() const { return m_WasCollected; };

protected:
	bool m_WasCollected;
};