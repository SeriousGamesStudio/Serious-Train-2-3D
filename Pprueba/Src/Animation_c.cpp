#include "Animation_c.h"
#include "Entity.h"

Animation_c::Animation_c():
	Component(ComponentType::ANIMATION)
	
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
	setAnimation("swim");
}

void Animation_c::update()
{
	currentState->addTime(0.16f);
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

	

	//animStateSwim_ = mesh->getSceneNode()->getCreator()->createAnimationState("animFishSwim"); pa
	
}


