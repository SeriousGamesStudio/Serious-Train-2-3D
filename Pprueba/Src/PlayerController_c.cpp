#include "PlayerController_c.h"
#include "Entity.h"
#include "RigidBody_c.h"
#include "Components.h"
#include "InputManager.h"

#define W 17
#define S 31
#define A 30
#define D 32

PlayerController_c::PlayerController_c() :
	Component(ComponentType::PLAYERCONTROLLER)
{
	std::cout << "constructora";
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
	switch (arg.key)
	{
	case W:
		sendMsg(new Msg::Move(_myEntity->getId(), Msg_Base::self, Msg::Move::Dir::FOREWARD));
		break;
	case S:
		sendMsg(new Msg::Move(_myEntity->getId(), Msg_Base::self, Msg::Move::Dir::BACKWARD));
		break;
	case A:
		sendMsg(new Msg::Move(_myEntity->getId(), Msg_Base::self, Msg::Move::Dir::LEFT));
		break;
	case D:
		sendMsg(new Msg::Move(_myEntity->getId(), Msg_Base::self, Msg::Move::Dir::RIGHT));
		break;
	default:
		break;
	}
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
	/*RigidBody_c * rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
	if (rb) {
		rb->get()->applyCentralImpulse(btVector3(0, 10, 0));
	}*/
	return false;
}

bool PlayerController_c::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return false;
}
