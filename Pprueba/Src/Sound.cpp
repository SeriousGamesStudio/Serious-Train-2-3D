#include "Sound.h"

Sound_c::Sound_c(SoundManager * soundManager_, const std::string & file, const irrklang::vec3df & position, const bool & loop)
{
	_sound = nullptr;
	soundManager = soundManager_;
	setSoundPosition(position);
	_loop = loop;
	setFile(file, _loop);
}

Sound_c::~Sound_c()
{
	if (_sound) {
		_sound->drop();
	}
}

void Sound_c::setFile(const std::string & file, const bool & loop)
{
	if (_sound) {
		_sound->drop();
	}
	_filename = file;
	_sound = soundManager->getSound(_filename, _position, loop);
}

bool Sound_c::isPlaying(void)
{
	if (!_sound)
		return false;
	return !_sound->getIsPaused();
}

bool Sound_c::isLoop(void)
{
	if (!_sound)
		return false;
	return _sound->isLooped();
}

bool Sound_c::isFinished(void)
{
	if (!_sound)
		return false;
	return _sound->isFinished();
}

void Sound_c::restartSound(void)
{
	_sound->setPlayPosition(irrklang::ik_u32(0));
}

void Sound_c::stopSound(void)
{
	_sound->stop();
}

void Sound_c::playSound(void)
{
	if (_sound) {
		if (isPlaying()) 
			restartSound();
		if (isFinished())
			setFile(_filename, _loop);

		_sound->setIsPaused(false);
	}
}

void Sound_c::pauseSound(void)
{
	if (_sound)
		_sound->setIsPaused(true);
}

void Sound_c::setPlay(const bool & play)
{
	if (_sound) {
		_sound->setIsPaused(!play);
	}
}

void Sound_c::setLoop(const bool & loop)
{
	if (_sound)
		_sound->setIsLooped(loop);
}

void Sound_c::setVolume(const float & newVolume)
{
	if(_sound)
		_sound->setVolume(newVolume);
}

float Sound_c::getVolume(void)
{
	if (!_sound) {
		return 0.f;
	}
	return _sound->getVolume();
}

irrklang::vec3df Sound_c::getSoundPosition(void)
{
	if (!_sound) {
		return { 0,0,0 };
	}
	return _position;
}

void Sound_c::setSoundPosition(const float & x, const float & y, const float & z)
{
	setSoundPosition(irrklang::vec3df(x, y, z));
}

void Sound_c::setSoundPosition(const irrklang::vec3df & newPos)
{
	if (_sound) {
		_sound->setPosition(_position);
	}
	_position = newPos;
}
