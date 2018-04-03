#ifndef _H_CAMERACONTROLLER_C_H_
#define _H_CAMERACONTROLLER_C_H_

#include "Component.h"
#include "InputManager.h"
#include "Camera_c.h"
#include "Notifications.h"

class CameraController_c:
	public Component, 
	public OIS::MouseListener, 
	public OIS::KeyListener
{
public:
	CameraController_c(Entity * entity, InputManager * inputManager);
	~CameraController_c();
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);


private:
	InputManager * inputManager_;
};
#endif // !_H_CAMERACONTROLLER_C_H_
