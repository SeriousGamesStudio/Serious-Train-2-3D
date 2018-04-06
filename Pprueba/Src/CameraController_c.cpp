#include "CameraController_c.h"
#include "Entity.h"
#include "Components.h"
#include "Camera_c.h"

#define MOUSE_SENSIBILITY 1000.f

CameraController_c::CameraController_c(InputManager * inputManager) :
	Component(ComponentType::CAMERACONTROLLER), inputManager_(inputManager)
{
	inputManager->addMouseListener(this, "ratonCamara");
	inputManager->addKeyListener(this, "teclaCamara");
}

CameraController_c::~CameraController_c()
{
	inputManager_->removeMouseListener(this);
	inputManager_->removeKeyListener(this);
}

bool CameraController_c::keyPressed(const OIS::KeyEvent & arg)
{	
	
	return false;
}

bool CameraController_c::keyReleased(const OIS::KeyEvent & arg)
{
	
	return false;
}

bool CameraController_c::mouseMoved(const OIS::MouseEvent & arg)
{
	
	sendMsg(new Msg::CameraOrientate(_myEntity->getId(), Msg_Base::self, (float)arg.state.X.rel/ MOUSE_SENSIBILITY, (float) arg.state.Y.rel /MOUSE_SENSIBILITY));
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


