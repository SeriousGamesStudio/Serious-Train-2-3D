#include "RigidBody_c.h"



RigidBody_c::RigidBody_c(Entity* entity, PhysicsManager* physicsManager, btRigidBody::btRigidBodyConstructionInfo info) :
	Component(entity, "RigidBody"), btRigidBody(info), _pM(physicsManager)
{
}

RigidBody_c::~RigidBody_c()
{
}

void RigidBody_c::start()
{
	_pM->addRigidBody(this);
}
