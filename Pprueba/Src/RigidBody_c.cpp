#include "RigidBody_c.h"
#include "Entity.h"
#include "Components.h"


RigidBody_c::RigidBody_c(float mass) :
	Component(ComponentType::RIGIDBODY), rb(nullptr)
{
}

RigidBody_c::~RigidBody_c()
{
	PhysicsManager::getInstance()->removeRigidBody(rb);
	delete rb->getMotionState();
	delete rb->getCollisionShape();
	delete rb;
}

void RigidBody_c::start()
{
	btTransform t = static_cast<Transform_c*>(_myEntity->getComponent(ComponentType::TRANSFORM))->get();
	btDefaultMotionState* dms = new btDefaultMotionState(t);
	//Esto puede provocar errores de dependecias si Collider no ha sido inicializado para que tome el transform
	btCollisionShape* cs = static_cast<Collider_c*>(_myEntity->getComponent(ComponentType::COLLIDER))->getCollisionShape();
	rb = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(_mass, dms, cs));
	PhysicsManager::getInstance()->addRigidBody(rb);
}

void RigidBody_c::update()
{
	{//Update position
		btTransform trans;
		rb->getMotionState()->getWorldTransform(trans);

		float x = trans.getOrigin().getX();
		float y = trans.getOrigin().getY();
		float z = trans.getOrigin().getZ();

		sendMsg(new Msg::ChangePosition(_myEntity->getId(), Msg_Base::self, x, y, z));
	}
}