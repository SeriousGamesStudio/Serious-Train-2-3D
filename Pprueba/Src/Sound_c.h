#ifndef _H_SOUND_C_H_
#define _H_SOUND_C_H_
#include "Component.h"
#include "Sound.h"
class Sound_c: public Component, public Sound
{
public:
	Sound_c(const std::string & file = std::string("OOf.mp3"), const bool & loop = false);
	virtual ~Sound_c();

	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base* msg);


private:

};
#endif // !_H_SOUND_C_H_
