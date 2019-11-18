#include <Input.h>

Input::Input() 
{
	setAllFalse();
}
Input::~Input() {}

void Input::setAllFalse()
{
	m_up = false;
	m_upPressed = false;
	m_down = false;
	m_left = false;
	m_right = false;
	m_spacePressed = false;
}
