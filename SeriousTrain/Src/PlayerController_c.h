#ifndef _H_PLAYERCONTROLLER_C_H_
#define _H_PLAYERCONTROLLER_C_H_

#include "Component.h"
#include "Components.h"
#include "Notifications.h"
#include "InputManager.h"
#include "btBulletDynamicsCommon.h"


class Transform_c;
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
	Transform_c* transform;
	Camera_c* cam;
	btVector3 lookingAt, walkingTo;
	OIS::KeyCode toggleMouseKey;
private:
	void updateMovementDirection();
};

#endif