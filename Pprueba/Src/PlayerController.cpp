#include "PlayerController.h"
#include "Entity.h"
#include "RigidBody_c.h"
#include "Components.h"
PlayerController_c::PlayerController_c() :
	Component(ComponentType::PLAYERCONTROLLER)
{
	InputManager::getInstance()->addKeyListener(this, "tecladoJugador");
	InputManager::getInstance()->addMouseListener(this, "ratonJugador");
}

PlayerController_c::~PlayerController_c()
{
	InputManager::getInstance()->removeKeyListener(this);
	InputManager::getInstance()->removeMouseListener(this);
}

bool PlayerController_c::keyPressed(const OIS::KeyEvent & arg)
{
	return true;
}

bool PlayerController_c::keyReleased(const OIS::KeyEvent & arg)
{
	return true;
}

bool PlayerController_c::mouseMoved(const OIS::MouseEvent & arg)
{
	return true;

}

bool PlayerController_c::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	RigidBody_c * rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
	if (rb) {
		rb->get()->applyCentralImpulse(btVector3(0, 10, 0));
	}
	return true;
}

bool PlayerController_c::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return true;
}
