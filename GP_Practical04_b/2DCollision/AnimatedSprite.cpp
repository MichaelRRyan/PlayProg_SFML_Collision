#include <AnimatedSprite.h>

AnimatedSprite::AnimatedSprite() 
{
	m_current_frame = 0;
}

AnimatedSprite::AnimatedSprite(const sf::Texture& t) :
	Sprite(t),
	m_animationIndex(0),
	m_current_frame(0),
	m_time(seconds(0.18f))
{
}

AnimatedSprite::AnimatedSprite(const sf::Texture& t, const sf::IntRect& rect) :
	Sprite(t),
	m_animationIndex(0),
	m_current_frame(0),
	m_time(seconds(0.18f))
	
{
	m_frames.push_back(rect);
}

AnimatedSprite::~AnimatedSprite() {}

const sf::Clock& AnimatedSprite::getClock() {
	return m_clock;
}

const sf::Time& AnimatedSprite::getTime() {
	return m_time;
}

const vector<IntRect>& AnimatedSprite::getFrames() {
	return m_frames;
}

const IntRect& AnimatedSprite::getFrame(int n) {
	return m_frames[n];
}

void AnimatedSprite::addFrame(IntRect& frame) {
	m_frames.push_back(frame);
}

const int AnimatedSprite::getCurrentFrame() {
	return m_current_frame;
}

void AnimatedSprite::update(){
	if (m_clock.getElapsedTime() > m_time) {
		if (m_animationIndex * 6 + 6 > m_current_frame + 1) // Go to next frame while there is still a next frame in the current animation
		{
			m_current_frame++;
		}
		else {
			m_current_frame = m_animationIndex * 6; // Loop back to the first frame when finished first loop of animation
		}
		m_clock.restart();
	}
}

void AnimatedSprite::changeAnimation(Animation t_animation)
{
	// Sets the animation index to the integer value of the passed in enum
	m_animationIndex = static_cast<int>(t_animation);
	m_current_frame = m_animationIndex * 6; // Sets the current frame of the animation to the first of this index
}

