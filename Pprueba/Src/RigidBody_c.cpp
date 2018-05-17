#include "RigidBody_c.h"
#include "Entity.h"
#include "Components.h"


RigidBody_c::RigidBody_c(btRigidBody::btRigidBodyConstructionInfo info) :
	Component(ComponentType::RIGIDBODY), rb(btRigidBody(info))
{
	rb.setSleepingThresholds(0, 0);
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
	this->transform = static_cast<Transform_c*>(_myEntity->getComponent(ComponentType::TRANSFORM));
	if (!this->transform) throw exception("Esta entidad no contiene un transform\n");
}

void RigidBody_c::update()
{
	btTransform xform;
	xform.setIdentity();
	xform = rb.getWorldTransform();
	transform->set(xform);
}