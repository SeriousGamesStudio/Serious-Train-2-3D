#ifndef _H_ANIMATION_H_
#define _H_ANIMATION_H_
#include "GraphicsManager.h"
#include "Component.h"
#include "Components.h"
class GraphicsManager;

class Animation_c:
	public Component
{
public:
	Animation_c(GraphicsManager* graphicsMgr);
	~Animation_c();

	void start();
	virtual void update(const Ogre::FrameEvent & evt);
	//ANIMATIONS
	void shakeAnimation();

private:
	GraphicsManager* graphicsMgr_;

	//ANIMATION SET
	Ogre::AnimationState* animStateShake_;
	Ogre::Animation* shake_;
};

#endif // !_H_ANIMATION_H_
