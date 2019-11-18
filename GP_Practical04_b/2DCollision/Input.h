#ifndef INPUT_H
#define INPUT_H

class Input
{
public:
	Input();
	~Input();

	void setAllFalse();

	bool m_up;
	bool m_upPressed;
	bool m_down;
	bool m_left;
	bool m_right;
	bool m_spacePressed;
};
#endif