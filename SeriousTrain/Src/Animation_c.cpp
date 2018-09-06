#include "Animation_c.h"
#include "Entity.h"

Animation_c::Animation_c(std::string animName):
	Component(ComponentType::ANIMATION),
	animName_(animName)
	
{
	
	currentState = nullptr;
	
	

}

Animation_c::~Animation_c()
{

}

void Animation_c::start()
{
	mesh = static_cast<MeshRenderer_c*>(_myEntity->getComponent(ComponentType::MESHRENDERER));
	_ogreEntity = static_cast<Ogre::Entity*>(mesh->getSceneNode()->getAttachedObject(0));
	setAnimation(animName_);
	
}

void Animation_c::update()
{ 
	if(currentState) currentState->addTime(0.05f);
} 

void Animation_c::setAnimation(std::string animName)
{
	if (currentState) { // si hay una puesta previamente se para
		currentState->setLoop(false);
		currentState->setEnabled(false);
	}
	currentState = _ogreEntity->getAnimationState(animName);  

	if (currentState) { // se activa la que se asigna si hay alguna 
		currentState->setLoop(true);
		currentState->setEnabled(true);
	} 

	
	
}


