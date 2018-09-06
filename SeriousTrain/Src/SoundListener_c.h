#ifndef _H_SOUNDLISTENER_C_H_
#define _H_SOUNDLISTENER_C_H_
#include "Component.h"


class SoundListener_c: public Component
{
public:
	SoundListener_c();
	virtual ~SoundListener_c();

	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base* msg);


private:
	Ogre::Vector3 position;
	Ogre::Vector3 lookAt;


};
#endif // !_H_SOUND_C_H_
