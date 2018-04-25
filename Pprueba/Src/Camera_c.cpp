#include "Camera_c.h"
#include "Entity.h"
#include "GraphicsManager.h"
#include "Components.h"
#include <iostream>
#define UMBRAL_ROTACION 0.8

Camera_c::Camera_c(): 
	Component(ComponentType::CAMERA)
{
	cam = GraphicsManager::getInstance()->getCamera();
	//camNode_ = graphicsManager_->createNewNode(NULL);

}

Camera_c::~Camera_c()
{

}

void Camera_c::update()
{
}

void Camera_c::setPosition(float x, float y, float z)
{
	cam->setPosition(x, y, z);
}

void Camera_c::rotateCamera(float x, float y, float z)
{
	
	cam->yaw((Ogre::Radian)x);
	if ( y > 0 && cam->getDirection().y < UMBRAL_ROTACION || y < 0 && cam->getDirection().y > -UMBRAL_ROTACION) 
		cam->pitch((Ogre::Radian)y);

	if (z != 0) cam->roll((Ogre::Radian)z);
	sendMsg( new Msg::LookAt( _myEntity->getId(), Msg_Base::self, cam->getDirection().x, cam->getDirection().y, cam->getDirection().z));
}
