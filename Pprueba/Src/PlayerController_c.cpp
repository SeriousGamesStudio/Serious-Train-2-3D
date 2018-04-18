#include "PlayerController_c.h"
#include "Entity.h"
#include "RigidBody_c.h"
#include "Components.h"
#include "Notifications.h"
#include <math.h>
#include "InputManager.h"
#include <iostream>

#define PI 3.14159265358f

PlayerController_c::PlayerController_c() :
	Component(ComponentType::PLAYERCONTROLLER, "PlayerController"),
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
	//No entra
	if (arg.key == forwardKey)	forward = true;
	if (arg.key == backKey)		back	= true;
	if (arg.key == rightKey)	right	= true;
	if (arg.key == leftKey)		left	= true;
	//Poner aquí breakpoint y cambiar a la aplicación. 
	//Se pulsan las teclas y no salta el breakpoint, así que no entra a la función
	updateMovementDirection();
	return true;
}

bool PlayerController_c::keyReleased(const OIS::KeyEvent & arg)
{
	//No entra
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
		break;
	}
	default:
		break;
	}
}

void PlayerController_c::updateMovementDirection()
{
	bool f = forward;
	bool b = back;
	bool r = right;
	bool l = left;

	if (f && b)	f = b = false;
	if (l && r)	r = l = false;

	//If no direction return
	if (!f && !b && !r && !l) {
		walker->setDirection(0, 0);
		return;
	}
	float angle = 0.0f;

	{//Get rotation angle
	/*SIMPLE MOVES*/
		//foward
			 if (f && !r && !l)/*angle = 0*/;		
		//back
		else if (b && !r && !l) angle = PI;
		//right
		else if (!f && !b && r) angle = PI * -0.5f;
		//left
		else if (!f && !b && l)	angle = PI *  0.5f;
	/*COMPOSE MOVES*/
		//foward-right
		else if (f && r)		angle = PI * -0.25f;
		//foward-left
		else if (f && l)		angle = PI *  0.25f;
		//back-right
		else if (b && r)		angle = PI * -0.75f;
		//back-left
		else if (b && l)		angle = PI *  0.75f;
	}
	printf("*************************************************************\n");
	printf("Camera looking at: %f, %f, %f\n", lookingAt.x(), lookingAt.y(), lookingAt.z());
	walkingTo = lookingAt.rotate({ 0,1,0 }, angle);
	printf("Walking to:        %f, %f, %f\n", walkingTo.x(), walkingTo.y(), walkingTo.z());
	walker->setDirection(walkingTo.x(), walkingTo.z());
}
