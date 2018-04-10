#include "Animation_c.h"


Animation_c::Animation_c(GraphicsManager* graphicsMgr):
	Component(ComponentType::ANIMATION),
	graphicsMgr_(graphicsMgr)
	
{
	start();

}

Animation_c::~Animation_c()
{
}

void Animation_c::start()
{
	shakeAnimation();
}

void Animation_c::update(const Ogre::FrameEvent & evt)
{
	animStateShake_->addTime(evt.timeSinceLastFrame);
}

void Animation_c::shakeAnimation()
{

	animStateShake_ = graphicsMgr_->getInstance()->getSceneManager()->getEntity("robot")->getAnimationState("swim");
	animStateShake_->setLoop(true);
	animStateShake_->setEnabled(true);
	
}


