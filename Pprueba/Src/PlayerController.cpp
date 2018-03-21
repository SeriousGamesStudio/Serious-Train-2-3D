#include "PlayerController.h"

PlayerController_c::PlayerController_c(Entity * entity, InputManager * inputManager_) : Component(entity, "PlayerController"), inputManager(inputManager_)
{
	inputManager->addKeyListener(this, "tecladoJugador");
	inputManager->addMouseListener(this, "ratonJugador");
}

PlayerController_c::~PlayerController_c()
{
	inputManager->removeKeyListener(this);
	inputManager->removeMouseListener(this);
}

bool PlayerController_c::keyPressed(const OIS::KeyEvent & arg)
{
	return false;
}

bool PlayerController_c::keyReleased(const OIS::KeyEvent & arg)
{
	return false;
}

bool PlayerController_c::mouseMoved(const OIS::MouseEvent & arg)
{
	return false;
}

bool PlayerController_c::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return false;
}

bool PlayerController_c::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return false;
}
