#include "Entity.h"
#include "Components.h"
#include "Game.h"
btVector3 aux1;
float auxX = 0;
float auxY = 0;
float auxZ = 0;

const float VELOCITY = 50.0f;

Walker_c::Walker_c():
	Component(ComponentType::WALKER, "Walker"), currentDirection(btVector3(0,0,0)), rb(nullptr)
{
	aux1 = btVector3(currentDirection);
	trans = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0));
}

Walker_c::~Walker_c()
{
	//Has no dynamic memory allocated
}

void Walker_c::start() {
	rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
#ifdef _DEBUG
	if (!rb)
		printf("The entity has no 'RigidBody' component!\n");
#endif // _DEBUG

	trans = rb->get()->getWorldTransform();
}

void Walker_c::update()
{
	// Aplicar una fuerza al rigidBody de la entidad
	// Cómo aplicamos las fuerza para que sean constantes??
	// Esto lo deberíamos haber aprendido en física, peero no lo hemos hecho
	if (currentDirection != aux1) {
		aux1 = currentDirection;

		printf("------------------------------------------------------\n");
		printf("Player Direction: %f, %f, %f\n", currentDirection.getX(), currentDirection.y(), currentDirection.z());
		printf("------------------------------------------------------\n");
	}

	trans.getOrigin().setX(trans.getOrigin().getX() + (currentDirection.getX() * VELOCITY * Game::getInstance()->getDeltaTime()));
	trans.getOrigin().setY(trans.getOrigin().getY() + (currentDirection.getY() * VELOCITY * Game::getInstance()->getDeltaTime()));
	trans.getOrigin().setZ(trans.getOrigin().getZ() + (currentDirection.getZ() * VELOCITY * Game::getInstance()->getDeltaTime()));

	rb->get()->setWorldTransform(trans);

	btVector3 d = rb->get()->getCenterOfMassPosition();
	if (d.getX() != auxX || d.getY() != auxY || d.getZ() != auxZ) {
		auxX = d.getX();
		auxY = d.getY();
		auxZ = d.getZ();
		printf("Player position: %f, %f, %f\n", d.getX(), d.getY(), d.getZ());
	}
}

void Walker_c::listen(Msg_Base * msg)
{
	//No necesita mensajes
}

void Walker_c::setDirection(float x, float z)
{
	currentDirection.setValue(x, 0, z);
#ifdef _DEBUG
	if (!currentDirection.isZero())
#endif // _DEBUG
		currentDirection.normalize();
}
