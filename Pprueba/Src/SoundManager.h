#ifndef _H_SOUNDMANAGER_H_
#define _H_SOUNDMANAGER_H_
#include <irrKlang.h>
#include <Ogre.h>
#include <string.h>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	bool initialise(void);
	void setListenerPosition(const Ogre::Vector3 &position, const Ogre::Vector3 &lookAt);
	irrklang::ISound * getSound(const std::string & filename, const irrklang::vec3df & position, const bool & loop);
	inline bool isInitialised() { return initialised; };
private:
	irrklang::ISoundEngine* engine;
	bool initialised;
};
#endif