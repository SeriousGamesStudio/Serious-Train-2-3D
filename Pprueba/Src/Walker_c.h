#ifndef _H_WALKER_C_H_
#define _H_WALKER_C_H_

#include "Component.h"
#include "BulletDynamics\Character\btKinematicCharacterController.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

class Transform_c;
class Walker_c :
	public Component, public btKinematicCharacterController
{
public:
	Walker_c(float velocity, btConvexShape* shape = nullptr);
	~Walker_c();

	virtual void start();
	virtual void update();

	void setDirection(float x, float z);

private:
	float _velocity;

};
#endif // !_H_CAMERA_C_H_