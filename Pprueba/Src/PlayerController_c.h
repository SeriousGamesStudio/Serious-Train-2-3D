#pragma once
#include "Component.h"
#include "Notifications.h"
#include "InputManager.h"
#include "btBulletDynamicsCommon.h"


class RigidBody_c;
class Walker_c;
class Camera_c;
class PlayerController_c: public Component, public OIS::MouseListener, public OIS::KeyListener
{
public:
	PlayerController_c();
	~PlayerController_c();
	virtual bool keyPressed(const OIS::KeyEvent &arg) ;
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg) { return true; };
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base*msg);

private:
	bool forward, back, right, left; 
	OIS::KeyCode forwardKey;
	OIS::KeyCode backKey;
	OIS::KeyCode rightKey;
	OIS::KeyCode leftKey;
	Walker_c* walker;
	RigidBody_c* rb;
	Camera_c* cam;
	btVector3 lookingAt, walkingTo;
private:
	void updateMovementDirection();
};

