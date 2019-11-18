#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\System\Clock.hpp>
#include <vector>
#include <Debug.h>

using namespace std;
using namespace sf;

enum class Animation
{
	Idle = 0,
	Jumping = 1,
	Falling = 2,
	Landing = 3,
	Walking = 4,
	Climbing = 5
};

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(const Texture&);
	AnimatedSprite(const Texture&, const IntRect&);
	~AnimatedSprite();

	const Clock& getClock();
	const Time& getTime();
	const vector<IntRect>& getFrames();
	const IntRect& getFrame(int);
	void addFrame(IntRect&);
	const int getCurrentFrame();
	void update();
	void changeAnimation(Animation t_animation);
	
private:
	Clock m_clock;
	Time m_time;
	vector<IntRect> m_frames;
	int m_current_frame;
	int m_animationIndex;
};

#endif // !ANIMATED_SPRITE_H
