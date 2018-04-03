#include "CameraController_c.h"
#include "Entity.h"
#include "Components.h"
#include "Camera_c.h"

#define W 17
#define S 31
#define A 30
#define D 32

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
	case W:
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
	//cam->getCamera()->rotate(cam->getCamera()->getPosition().getRotationTo(Ogre::Vector3(0,0,0)));
	//cam->getCamera()->setOrientation(Ogre::Quaternion::IDENTITY);
	
	
	return false;
}

bool CameraController_c::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return false;
}

bool CameraController_c::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return false;
}


