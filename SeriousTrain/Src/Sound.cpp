#include "Sound.h"

Sound::Sound(const std::string & file, const irrklang::vec3df & position, const bool & loop)
{
	_sound = nullptr;
	setSoundPosition(position);
	_loop = loop;
	setFile(file, _loop);
}

Sound::~Sound()
{
	if (_sound) {
		_sound->drop();
	}
}

void Sound::setFile(const std::string & file, const bool & loop)
{
	if (_sound) {
		_sound->drop();
	}
	_filename = file;
	_sound = SoundManager::getInstance()->getSound(_filename, _position, loop);
}

bool Sound::isPlaying(void)
{
	if (!_sound)
		return false;
	return !_sound->getIsPaused();
}

bool Sound::isLoop(void)
{
	if (!_sound)
		return false;
	return _sound->isLooped();
}

bool Sound::isFinished(void)
{
	if (!_sound)
		return false;
	return _sound->isFinished();
}

void Sound::restartSound(void)
{
	_sound->setPlayPosition(irrklang::ik_u32(0));
}

void Sound::stopSound(void)
{
	_sound->stop();
}

void Sound::playSound(void)
{
	if (_sound) {
		if (isPlaying()) 
			restartSound();
		if (isFinished())
			setFile(_filename, _loop);

		_sound->setIsPaused(false);
	}
}

void Sound::pauseSound(void)
{
	if (_sound)
		_sound->setIsPaused(true);
}

void Sound::setPlay(const bool & play)
{
	if (_sound) {
		_sound->setIsPaused(!play);
	}
}

void Sound::setLoop(const bool & loop)
{
	if (_sound)
		_sound->setIsLooped(loop);
}

void Sound::setVolume(const float & newVolume)
{
	if(_sound)
		_sound->setVolume(newVolume);
}

float Sound::getVolume(void)
{
	if (!_sound) {
		return 0.f;
	}
	return _sound->getVolume();
}

irrklang::vec3df Sound::getSoundPosition(void)
{
	if (!_sound) {
		return { 0,0,0 };
	}
	return _position;
}

void Sound::setSoundPosition(const float & x, const float & y, const float & z)
{
	setSoundPosition(irrklang::vec3df(x, y, z));
}

void Sound::setSoundPosition(const irrklang::vec3df & newPos)
{
	if (_sound) {
		_sound->setPosition(_position);
	}
	_position = newPos;
}
