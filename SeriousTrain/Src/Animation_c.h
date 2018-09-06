#ifndef _H_ANIMATION_H_
#define _H_ANIMATION_H_
#include "GraphicsManager.h"
#include "Component.h"
#include "Components.h"

class Animation_c:
	public Component
{
public:
	Animation_c(std::string animName);
	~Animation_c();

	virtual void start();
	virtual void update();
	//ANIMATIONS
	void setAnimation(std::string animName);

private:
	MeshRenderer_c * mesh;
	Ogre::Entity* _ogreEntity;
	std::string animName_;
	//ANIMATION SET
	Ogre::AnimationState* currentState;

	
};

#endif // !_H_ANIMATION_H_
