#pragma once
#include "Component.h"
#include "InputManager.h"
class PlayerController_c: public Component, public OIS::MouseListener, public OIS::KeyListener
{
public:
	PlayerController_c(Entity * entity);
	~PlayerController_c();
	virtual bool keyPressed(const OIS::KeyEvent &arg) ;
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
private:
	InputManager * inputManager;
};

