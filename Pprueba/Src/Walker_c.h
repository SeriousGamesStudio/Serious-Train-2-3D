#ifndef _H_WALKER_C_H_
#define _H_WALKER_C_H_

#include "Component.h"


class RigidBody_c;
class Walker_c:
	public Component
{
public:
	Walker_c(float velocity);
	~Walker_c();

	virtual void start();
	virtual void update();

	void setDirection(float x, float z);

private:
	float _velocity;
	RigidBody_c* rb;
	btVector3 direction;

};
#endif // !_H_CAMERA_C_H_