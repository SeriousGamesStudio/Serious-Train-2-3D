#include "Entity.h"
#include "Components.h"
#include "Game.h"
#include "Transform_c.h"


#ifdef _DEBUG
btVector3 aux1;
float auxX = 0;
float auxY = 0;
float auxZ = 0;
#endif // _DEBUG

const float VELOCITY = 50.0f;

Walker_c::Walker_c():
	Component(ComponentType::WALKER), currentDirection(btVector3(0,0,0)), rb(nullptr)
{
	aux1 = btVector3(currentDirection);
}

Walker_c::~Walker_c()
{
	//Has no dynamic memory allocated
}

void Walker_c::start() {
	trans = static_cast<Transform_c*>(_myEntity->getComponent(ComponentType::TRANSFORM));
#ifdef _DEBUG

#endif // _DEBUG

}

void Walker_c::update()
{
#ifdef _DEBUG
	if (currentDirection != aux1) {
		aux1 = currentDirection;

		printf("------------------------------------------------------\n");
		printf("Player Direction: %f, %f, %f\n", currentDirection.getX(), currentDirection.y(), currentDirection.z());
		printf("------------------------------------------------------\n");
	}
#endif // _DEBUG

	trans->get() = updateTransform();

#ifdef _DEBUG
	btVector3 d = trans->get().getOrigin();
	if (d.getX() != auxX || d.getY() != auxY || d.getZ() != auxZ) {
		auxX = d.getX();
		auxY = d.getY();
		auxZ = d.getZ();
		printf("Player position: %f, %f, %f\n", d.getX(), d.getY(), d.getZ());
	}
#endif // _DEBUG
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

btTransform& Walker_c::updateTransform()
{
	trans->get().getOrigin().setX(trans->get().getOrigin().getX() + (currentDirection.getX() * VELOCITY * Game::getInstance()->getDeltaTime()));
	trans->get().getOrigin().setY(trans->get().getOrigin().getY() + (currentDirection.getY() * VELOCITY * Game::getInstance()->getDeltaTime()));
	trans->get().getOrigin().setZ(trans->get().getOrigin().getZ() + (currentDirection.getZ() * VELOCITY * Game::getInstance()->getDeltaTime()));

	return trans->get();
}
