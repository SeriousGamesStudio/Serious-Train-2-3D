#include "SoundManager.h"
using namespace irrklang;
SoundManager::SoundManager()
{
	initialised = false;
	engine = nullptr;
}

SoundManager::~SoundManager()
{
	engine->drop();
}

bool SoundManager::initialise(void)
{
	engine = createIrrKlangDevice();
	if (engine)
		initialised = true;
	return initialised;
}

void SoundManager::setListenerPosition(const Ogre::Vector3 &position, const Ogre::Vector3 &lookAt)
{
	vec3df pos = vec3df(position.x, position.y, position.z);
	vec3df look = vec3df(lookAt.x, lookAt.y, lookAt.z);

	engine->setListenerPosition(pos, look);
}

irrklang::ISound * SoundManager::getSound(const std::string & filename, const irrklang::vec3df & position, const bool & loop)
{
	std::string file = "../Media/Sounds/" + filename;

	return engine->play3D(file.c_str(),position,loop,true);
}
