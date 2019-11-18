#include <PlayerFSM.h>
#include <Idle.h>

PlayerFSM::PlayerFSM()
{
	m_current = new Idle();
}

PlayerFSM::~PlayerFSM() {}

void PlayerFSM::setCurrent(State* s)
{
	m_current = s;
}

State* PlayerFSM::getCurrent()
{
	return m_current;
}

//void PlayerFSM::setPrevious(State* s)
//{
//	m_previous = s;
//}

//State* PlayerFSM::getPrevious()
//{
//	return m_previous;
//}

void PlayerFSM::handleInput(AnimatedSprite *t_animatedSprite, Input t_input)
{
	m_current->handleInput(this, t_animatedSprite, t_input); // Handle state input
}

void PlayerFSM::update(AnimatedSprite* t_animatedSprite)
{
	m_current->update(this, t_animatedSprite); // Update the state
}

void PlayerFSM::idle()
{
	m_current->idle(this);
}

void PlayerFSM::jumping()
{
	m_current->jumping(this);
}

void PlayerFSM::climbing()
{
	m_current->climbing(this);
}

void PlayerFSM::walking()
{
	m_current->walking(this);
}

void PlayerFSM::falling()
{
	m_current->falling(this);
}

void PlayerFSM::landing()
{
	m_current->landing(this);
}
