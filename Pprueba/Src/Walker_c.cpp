#include "Entity.h"
#include "Components.h"
#include "Game.h"
#include "Transform_c.h"
#include "PhysicsManager.h"



Walker_c::Walker_c(float velocity, btConvexShape* shape):
	Component(ComponentType::WALKER), btKinematicCharacterController(new btPairCachingGhostObject(), shape, 0.5), _velocity(velocity)
{
	
}

Walker_c::~Walker_c()
{
	//Has no dynamic memory allocated
}


void Walker_c::start()
{
	m_convexShape = (btConvexShape*)static_cast<Collider_c*>(getComponent(ComponentType::COLLIDER))->getCollisionShape();
}

void Walker_c::update()
{
	updateAction(PhysicsManager::getInstance()->getWorld(), Game::getInstance()->getDeltaTime());
}

void Walker_c::setDirection(float x, float z)
{
	setWalkDirection(btVector3(x, 0, z));
}
