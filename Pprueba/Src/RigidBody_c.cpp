#include "RigidBody_c.h"



RigidBody_c::RigidBody_c(Entity* entity, PhysicsManager* physicsManager, btRigidBody::btRigidBodyConstructionInfo info) :
	Component(entity), btRigidBody(info), pM(physicsManager)
{
}

RigidBody_c::~RigidBody_c()
{
}

void RigidBody_c::start()
{
	pM->addRigidBody(this);
}
