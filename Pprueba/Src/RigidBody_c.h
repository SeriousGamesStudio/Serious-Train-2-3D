#ifndef _H_RIGIDBODY_C_H_
#define _H_RIGIDBODY_C_H_

#include "Component.h"
#include "PhysicsManager.h"
#include "btBulletCollisionCommon.h"

class RigidBody_c
	:Component, btRigidBody
{
public:
	RigidBody_c(Entity* entity, PhysicsManager* physicsManager, btRigidBody::btRigidBodyConstructionInfo info);
	virtual ~RigidBody_c();


	virtual void start();

private:
	PhysicsManager * pM;

};
#endif //!_H_RIGIDBODY_C_H_

