#include "RigidBody_c.h"
#include "Entity.h"



RigidBody_c::RigidBody_c(Entity* entity, PhysicsManager* physicsManager, btRigidBody::btRigidBodyConstructionInfo info) :
	Component(entity, "RigidBody"), rb(btRigidBody(info)), _pM(physicsManager)
{
}

RigidBody_c::~RigidBody_c()
{
}

void RigidBody_c::start()
{
	_pM->addRigidBody(&rb);
}

void RigidBody_c::update()
{
	{//Update position
		btTransform trans;
		rb.getMotionState()->getWorldTransform(trans);

		float x = trans.getOrigin().getX();
		float y = trans.getOrigin().getY();
		float z = trans.getOrigin().getZ();

		sendMsg(new Msg::ChangePosition(_myEntity->getId(), Msg_Base::self, x, y, z));
	}
}