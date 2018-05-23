#ifndef _H_WALKER_C_H_
#define _H_WALKER_C_H_

#include "Component.h"
#include "Components.h"
#include "RigidBody_c.h"

class GraphicsManager;

class Transform_c;
class Walker_c :
	public Component
{
public:
	Walker_c();
	~Walker_c();

	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base* msg);

	void setDirection(float x, float z);
	void setVelocity(float vel);

private:
	btVector3 currentDirection;
	RigidBody_c* rb;
	Transform_c* trans;
	float _velocity;

private:
	btTransform& updateTransform();
};
#endif // !_H_CAMERA_C_H_