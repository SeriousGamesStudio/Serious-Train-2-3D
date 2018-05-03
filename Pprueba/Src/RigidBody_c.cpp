#include "RigidBody_c.h"
#include "Entity.h"
#include "Components.h"


RigidBody_c::RigidBody_c(btRigidBody::btRigidBodyConstructionInfo info) :
	Component(ComponentType::RIGIDBODY), rb(btRigidBody(info))
{
}

RigidBody_c::~RigidBody_c()
{
	PhysicsManager::getInstance()->removeRigidBody(&rb);
	delete rb.getMotionState();
	delete rb.getCollisionShape();
}

void RigidBody_c::start()
{
	PhysicsManager::getInstance()->addRigidBody(&rb);
}

void RigidBody_c::update()
{
	
}