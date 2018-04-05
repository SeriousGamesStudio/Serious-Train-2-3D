#include "CameraController_c.h"
#include "Entity.h"
#include "Components.h"
#include "Camera_c.h"

#define W 17
#define S 31
#define A 30
#define D 32
#define MOUSE_SENSIBILITY 1000.f

CameraController_c::CameraController_c(Entity * entity, InputManager * inputManager) :
	Component(entity, ComponentType::CAMERACONTROLLER), inputManager_(inputManager)
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

	switch (arg.key)
	{
	case OIS::KeyCode::KC_W:
		sendMsg(new Msg::CameraMove(_myEntity->getId(), Msg_Base::self,Msg::CameraMove::Move::FOREWARD));
		break;
	case S:
		sendMsg(new Msg::CameraMove(_myEntity->getId(), Msg_Base::self, Msg::CameraMove::Move::BACKWARD));
		break;
	case A:
		sendMsg(new Msg::CameraMove(_myEntity->getId(), Msg_Base::self, Msg::CameraMove::Move::LEFT));
		break;
	case D:
		sendMsg(new Msg::CameraMove(_myEntity->getId(), Msg_Base::self, Msg::CameraMove::Move::RIGHT));
		break;
	default:
		break;
	}
	
	return false;
}

bool CameraController_c::keyReleased(const OIS::KeyEvent & arg)
{
	switch (arg.key)
	{
	case W:
		sendMsg(new Msg::CameraStop(_myEntity->getId(), Msg_Base::self, Msg::CameraStop::Move::FOREWARD));
		break;
	case S:
		sendMsg(new Msg::CameraStop(_myEntity->getId(), Msg_Base::self, Msg::CameraStop::Move::BACKWARD));
		break;
	case A:
		sendMsg(new Msg::CameraStop(_myEntity->getId(), Msg_Base::self, Msg::CameraStop::Move::LEFT));
		break;
	case D:
		sendMsg(new Msg::CameraStop(_myEntity->getId(), Msg_Base::self, Msg::CameraStop::Move::RIGHT));
		break;
	default:
		break;
	}
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


