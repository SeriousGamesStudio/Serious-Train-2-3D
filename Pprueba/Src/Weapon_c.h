#ifndef _H_WEAPON_H_
#define _H_WEAPON_H_
#include "InputManager.h"
#include "Component.h"


class Weapon_c:
	public Component,
	public OIS::MouseListener
{
public:
	Weapon_c();
	~Weapon_c();


	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base*msg);

private:

	RigidBody_c * rb_;

};


#endif // !_H_WEAPON_H_
