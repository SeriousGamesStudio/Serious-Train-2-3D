#include "CameraController_c.h"
#include "Entity.h"
#include "Components.h"
#include "Camera_c.h"


CameraController_c::CameraController_c() :
	Component(ComponentType::CAMERACONTROLLER)
{
	InputManager::getInstance()->addMouseListener(this, "ratonCamara");

}

CameraController_c::~CameraController_c()
{
	InputManager::getInstance()->removeMouseListener(this);
}


void CameraController_c::start()
{
	cam = static_cast<Camera_c*>(_myEntity->getComponent(ComponentType::CAMERA));
}



bool CameraController_c::mouseMoved(const OIS::MouseEvent & arg)
{
	cam->rotateCamera((float)-arg.state.X.rel / 1000.f, (float)-arg.state.Y.rel / 1000.f);

	return true;
}

bool CameraController_c::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return true;
}

bool CameraController_c::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return true;
}


