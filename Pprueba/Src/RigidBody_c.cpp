#include "RigidBody_c.h"
#include "Entity.h"
#include "Components.h"


RigidBody_c::RigidBody_c(btRigidBody::btRigidBodyConstructionInfo info) :
	Component(ComponentType::RIGIDBODY, "RigidBody"), rb(btRigidBody(info))
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
	{//Update position
		btTransform trans;
		rb.getMotionState()->getWorldTransform(trans);

		float x = trans.getOrigin().getX();
		float y = trans.getOrigin().getY();
		float z = trans.getOrigin().getZ();

		sendMsg(new Msg::ChangePosition(_myEntity->getId(), Msg_Base::self, x, y, z));
	}
}