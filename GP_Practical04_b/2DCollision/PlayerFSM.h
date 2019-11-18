#ifndef PLAYER_FSM_H
#define PLAYER_FSM_H

#include <AnimatedSprite.h>
#include <string>
#include <Input.h>

class PlayerFSM
{

	// Please review very good article on Stackoverflow
	// which covers some solutions to circular dependacies
	// https://stackoverflow.com/questions/625799/resolve-build-errors-due-to-circular-dependency-amongst-classes

private:
	class State* m_current; // note order of m_current is 
							// prior to setCurrent
	//class State* m_previous;

	std::string m_currentName;
public:
	PlayerFSM();
	~PlayerFSM();
	void setCurrent(State* s);
	//void setPrevious(State* s);
	State* getCurrent();
	//State* getPrevious();
	std::string getCurrentName() { return m_currentName; }
	void setCurrentName(const std::string t_name) { m_currentName = t_name; };
	
	void handleInput(AnimatedSprite *t_animatedSprite, Input t_input);
	void update(AnimatedSprite* t_animatedSprite);
	void idle();
	void jumping();
	void climbing();
	void walking();
	void falling();
	void landing();

// Try uncommenting and comment the declaration above
//private:
//	class State* m_current;
};

#endif // !ANIMATION_H
