#include "Entity.h"
#include "Components.h"
#include "Game.h"
#include "Transform_c.h"
#include "PhysicsManager.h"



Walker_c::Walker_c(float velocity):
	Component(ComponentType::WALKER), _velocity(velocity), direction(btVector3(0,0,0))
{
	
}

Walker_c::~Walker_c()
{
}


void Walker_c::start()
{
	rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
	//rb->get()->setLinearFactor(btVector3(1, 0, 1));
	if (!rb) throw exception("No se ha encontrado RigidBody");
}

void Walker_c::update()
{
	btTransform xform;
	xform.setIdentity();
	xform = rb->get()->getWorldTransform();
	xform.getOrigin().setX(xform.getOrigin().getX() + (direction.getX() *_velocity * Game::getInstance()->getDeltaTime()));
	xform.getOrigin().setZ(xform.getOrigin().getZ() + (direction.getZ() * _velocity * Game::getInstance()->getDeltaTime()));
	//printf("Velocity: %i, %i, %i\n",direction.getX() *_velocity, direction.getZ() * _velocity);
	rb->get()->setWorldTransform(xform);
}

void Walker_c::setDirection(float x, float z)
{
	direction.setValue(x, 0, z);
	if (!direction.isZero()) direction.normalize();
}
