#ifndef _H_WALKER_C_H_
#define _H_WALKER_C_H_

#include "Component.h"
#include <Ogre.h>
class GraphicsManager;


class Walker_c :
	public Component
{
public:
	Walker_c();
	~Walker_c();

	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base* msg);

private:

	float dirX, dirZ;
	RigidBody_c* rb;
};
#endif // !_H_CAMERA_C_H_