#include "CameraController_c.h"
#include "Entity.h"
#include "RigidBody_c.h"
#include "Components.h"

CameraController_c::CameraController_c(Entity * entity, InputManager * inputManager, GraphicsManager* graphicsManager):
	Component(entity, ComponentType::PLAYERCONTROLLER), inputManager_(inputManager), graphicsManager_(graphicsManager)
{
	inputManager->addMouseListener(this, "ratonCamara");
	inputManager->addKeyListener(this, "teclaCamara");
	foreward = left = right = backward = false;
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
	case 17:
		foreward = true;		
		break;
	case 31:
		backward = true;
		break;
	case 30:
		left = true;
		break;
	case 32:
		right = true;
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
	case 17:
		foreward = false;		
		break;
	case 31:
		backward = false;
		break;
	case 30:
		left = false;
		break;
	case 32:
		right = false;
		break;
	default:
		break;
	}
	return false;
}

bool CameraController_c::mouseMoved(const OIS::MouseEvent & arg)
{
	//graphicsManager_->getCamera()->setPosition(arg.state.X.abs / 100, - arg.state.Y.abs / 100, graphicsManager_->getCamera()->getPosition().z);



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

void CameraController_c::update()
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
