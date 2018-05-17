#ifndef _H_RIGIDBODY_C_H_
#define _H_RIGIDBODY_C_H_

#include "Component.h"
#include "PhysicsManager.h"

class Transform_c;

class RigidBody_c :
	public Component
{
public:
	RigidBody_c(btRigidBody::btRigidBodyConstructionInfo info);
	virtual ~RigidBody_c();

	inline btRigidBody* get() { return &rb; };
	virtual void start();

	virtual void update();

private:
	PhysicsManager * _pM;
	btRigidBody rb;
	Transform_c* transform;
};
#endif //!_H_RIGIDBODY_C_H_

