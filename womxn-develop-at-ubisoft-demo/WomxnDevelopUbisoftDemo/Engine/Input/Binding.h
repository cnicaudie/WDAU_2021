#pragma once

class Binding
{
public:
	virtual int GetBinding() = 0;
};

/*class JoystickButtonBinding : public Binding
{
public:
	JoystickButtonBinding(const unsigned int& key);

	inline int GetBinding() override { return m_Button; };

private:
	unsigned int m_Button;
};*/