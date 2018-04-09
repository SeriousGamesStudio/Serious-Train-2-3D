#include "PlayerController_c.h"
#include "Entity.h"
#include "RigidBody_c.h"
#include "Components.h"
#include "Notifications.h"
#include <math.h>
#include "InputManager.h"

#define PI 3.141592653589793238463

PlayerController_c::PlayerController_c() :
	Component(ComponentType::PLAYERCONTROLLER),
	forward(false), back(false), right(false), left(false),
	forwardKey(OIS::KeyCode::KC_W), backKey(OIS::KeyCode::KC_S), rightKey(OIS::KeyCode::KC_D), leftKey(OIS::KeyCode::KC_A),
	lookingAt(btVector3(0,0,0)), walkingTo(btVector3(0,0,0))
{
	std::cout << "constructora";
	InputManager::getInstance()->addKeyListener(this, "tecladoJugador");
	InputManager::getInstance()->addMouseListener(this, "ratonJugador");
	//Change predefined keys if the user changes the configuration
}


void PlayerController_c::start() 
{

	walker = static_cast<Walker_c*>(_myEntity->getComponent(ComponentType::WALKER));
	rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
	cam = static_cast<Camera_c*>(_myEntity->getComponent(ComponentType::CAMERA));
#ifdef _DEBUG
	if (!walker)
		printf("ERROR: La entidad no contiene 'Walker' component!\n");
	if (!rb)
		printf("ERROR: La entidad no contiene 'RigidBody' component!\n");
	if (!cam)
		printf("ERROR: La entidad no contiene 'Camera' component!\n");
#endif // _DEBUG

}

void PlayerController_c::update() 
{
	//get rb position
	btVector3 rbPos = rb->get()->getCenterOfMassPosition();
	//add height of the player
	//set camera to that position
	cam->setPosition(rbPos.x(), rbPos.y(), rbPos.z());
}
PlayerController_c::~PlayerController_c()
{
	InputManager::getInstance()->removeKeyListener(this);
	InputManager::getInstance()->removeMouseListener(this);
}

bool PlayerController_c::keyPressed(const OIS::KeyEvent & arg)
{
	if (arg.key == forwardKey)	forward = true;
	if (arg.key == backKey)		back	= true;
	if (arg.key == rightKey)	right	= true;
	if (arg.key == leftKey)		left	= true;

	updateMovementDirection();
	return true;
}

bool PlayerController_c::keyReleased(const OIS::KeyEvent & arg)
{
	if (arg.key == forwardKey)	forward	= false;
	if (arg.key == backKey)		back	= false;
	if (arg.key == rightKey)	right	= false;
	if (arg.key == leftKey)		left	= false;

	updateMovementDirection();
	return true;
}


bool PlayerController_c::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	//Shoot!
	return true;
}

bool PlayerController_c::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	//Exit 
	return true;
}


void PlayerController_c::listen(Msg_Base * msg)
{
	switch (msg->id)
	{
	case MsgId::LOOKING_TO: 
	{
		Msg::LookAt* p = static_cast<Msg::LookAt*>(msg);
		lookingAt.setValue(p->x, 0, p->z);
		updateMovementDirection();
	}
	default:
		break;
	}
}

void PlayerController_c::updateMovementDirection()
{
	//Setting the vector from the seight
	btVector3 keyDir = btVector3(0,0,0);
	if (forward)
		keyDir.setZ(keyDir.z()+1);
	if (back)
		keyDir.setZ(keyDir.z() - 1);
	if (right)
		keyDir.setX(keyDir.x() + 1);
	if (left)
		keyDir.setX(keyDir.x() - 1);
	keyDir.normalize();

	walkingTo.setX(lookingAt.x() * keyDir.x());
	walkingTo.setZ(lookingAt.z() * keyDir.z());


	walker->setDirection(walkingTo.x(), walkingTo.z());
}
