#ifndef _H_WEAPON_H_
#define _H_WEAPON_H_
#include "InputManager.h"
#include "Component.h"

class Transform_c;
class Weapon_c:
	public Component,
	public OIS::MouseListener
{
public:
	Weapon_c(float range,int damage, float cadency);
	~Weapon_c();


	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseMoved(const OIS::MouseEvent &arg) { return true; };

	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base*msg);

private:

	float range_, cadency_;
	int dmg_;
	Transform_c *t;
	btVector3 aiming;
};


#endif // !_H_WEAPON_H_
