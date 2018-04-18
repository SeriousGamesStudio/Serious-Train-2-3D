#include "Camera_c.h"
#include "Entity.h"
#include "GraphicsManager.h"
#include "Components.h"
#include <iostream>

Camera_c::Camera_c(): 
	Component(ComponentType::CAMERA, "Camera")
{
	cam = GraphicsManager::getInstance()->getCamera();
}

Camera_c::~Camera_c()
{

}

void Camera_c::update()
{
	//Esto es MUY provisional
	float x = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY))->get()->getCenterOfMassPosition().getX();
	float y = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY))->get()->getCenterOfMassPosition().getY();
	float z = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY))->get()->getCenterOfMassPosition().getZ();
	//std::cout << "x: " << x << " y: " << y << " z " << z<<"\n";
	cam->setPosition(x, y, z);
}

void Camera_c::listen(Msg_Base * msg)
{
	
	Msg::CameraOrientate * o = static_cast<Msg::CameraOrientate*>(msg);
	switch (msg->id)		//tipo de mensaje
	{			
	case MsgId::CAMERA_ORIENTATE:
		cam->yaw((Ogre::Radian)o->degreesX);
		cam->pitch((Ogre::Radian)o->degreesY);
		sendMsg(new Msg::LookAt(_myEntity->getId(), Msg_Base::self, cam->getDirection().x, cam->getDirection().y, cam->getDirection().z));
		break;
	default:
		break;
	}
	
}

void Camera_c::setPosition(float x, float y, float z)
{
	cam->setPosition(x, y, z);
}
