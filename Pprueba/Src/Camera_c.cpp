#include "Camera_c.h"
#include "GraphicsManager.h"
#include "Components.h"
#include <iostream>

Camera_c::Camera_c(Entity * entity, GraphicsManager * graphicsManager): 
	Component(entity,ComponentType::CAMERA)
{
	graphicsManager_ = graphicsManager;
	//camNode_ = graphicsManager_->createNewNode(NULL);
	foreward = left = right = backward = false;

}

Camera_c::~Camera_c()
{

}

void Camera_c::update()
{
	if (foreward) {
		graphicsManager_->getCamera()->setPosition(graphicsManager_->getCamera()->getPosition().x, graphicsManager_->getCamera()->getPosition().y,
			graphicsManager_->getCamera()->getPosition().z - 1);
	}
	if (backward) {
		graphicsManager_->getCamera()->setPosition(graphicsManager_->getCamera()->getPosition().x, graphicsManager_->getCamera()->getPosition().y,
			graphicsManager_->getCamera()->getPosition().z + 1);
	}
	if (left) {
		graphicsManager_->getCamera()->setPosition(graphicsManager_->getCamera()->getPosition().x - 1, graphicsManager_->getCamera()->getPosition().y,
			graphicsManager_->getCamera()->getPosition().z);
	}
	if (right) {
		graphicsManager_->getCamera()->setPosition(graphicsManager_->getCamera()->getPosition().x + 1, graphicsManager_->getCamera()->getPosition().y,
			graphicsManager_->getCamera()->getPosition().z);
	}
}

void Camera_c::listen(Msg_Base * msg)
{
	Msg::CameraMove * p = static_cast<Msg::CameraMove*>(msg);
	Msg::CameraOrientate * o = static_cast<Msg::CameraOrientate*>(msg);
	switch (msg->id)		//tipo de mensaje
	{		
	case MsgId::CAMERA_MOVED: 
		std::cout << "¡tsuu";
		switch (p->info)		//info del tipo del mensaje
		{
		case Msg::CameraMove::Move::FOREWARD:
			foreward = true;
			break;
		case Msg::CameraMove::Move::BACKWARD:
			backward = true;
			break;
		case Msg::CameraMove::Move::LEFT:
			left = true;
			break;
		case Msg::CameraMove::Move::RIGHT:
			right = true;
			break;
		default:
			break;
		}
		break;
	case MsgId::CAMERA_STOPPED:
		std::cout << "¡tsuu2";

		switch (p->info)		//info del tipo del mensaje
		{
		case Msg::CameraMove::Move::FOREWARD:
			foreward = false;
			break;
		case Msg::CameraMove::Move::BACKWARD:
			backward = false;
			break;
		case Msg::CameraMove::Move::LEFT:
			left = false;
			break;
		case Msg::CameraMove::Move::RIGHT:
			right = false;
			break;
		default:
			break;
		}
		break;
	case MsgId::CAMERA_ORIENTATE:
		graphicsManager_->getCamera()->yaw((Ogre::Radian)o->degreesX);
		graphicsManager_->getCamera()->pitch((Ogre::Radian)o->degreesY);
		break;
	default:
		break;
	}
	
}
