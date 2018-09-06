#ifndef _H_RIGIDBODY_C_H_
#define _H_RIGIDBODY_C_H_

#include "Component.h"
#include "Components.h"
#include "PhysicsManager.h"

class RigidBody_c :
	public Component
{
public:
	RigidBody_c(float mass);
	virtual ~RigidBody_c();

	inline btRigidBody* get() { return rb; };
	virtual void start();

	virtual void update();

private:
	float _mass;
	PhysicsManager * _pM;
	btRigidBody* rb;
};
#endif //!_H_RIGIDBODY_C_H_

