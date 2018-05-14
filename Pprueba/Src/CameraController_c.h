#ifndef _H_CAMERACONTROLLER_C_H_
#define _H_CAMERACONTROLLER_C_H_

#include "Component.h"
#include "Components.h"
#include "InputManager.h"
class Camera_c;

class CameraController_c:
	public Component, 
	public OIS::MouseListener
{
public:
	CameraController_c();
	~CameraController_c();

	virtual void start();

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);


private:
	Camera_c * cam;
};
#endif // !_H_CAMERACONTROLLER_C_H_
