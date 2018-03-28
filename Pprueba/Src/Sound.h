#ifndef _H_SOUND_H_
#define _H_SOUND_H_
#include "SoundManager.h"
class Sound_c
{
public:
	//By default the sound the sound is unnasigned and it will be the death sound of roblox
	//and it will be in the position (0,0,0), and it wont be a loop
	Sound_c(SoundManager * soundManager,const std::string & file = std::string("OOf.mp3"), 
		const irrklang::vec3df & postion = irrklang::vec3df(0.f,0.f,0.f),const bool & loop=false);
	~Sound_c();
	
	// change the source of the sound
	void setFile(const std::string & file = std::string("OOf.mp3"), const bool & loop = false);

	// Self-explanatory code
	bool isPlaying(void);
	// Self-explanatory code
	bool isLoop(void);
	// Self-explanatory code
	bool isFinished(void);

	// Restart the sound but keep playing if is playing
	void restartSound(void);
	// Restart the sound but stop playing
	void stopSound(void);
	// Self-explanatory code
	void playSound(void);
	// Pause the sound but keep the current position of the sound.
	void pauseSound(void);

	// true for play, false for pause
	void setPlay(const bool & play);
	// true for loop the sound, false for not loop sound
	void setLoop(const bool & loop);

	// Self-explanatory code
	void setVolume(const float & newVolume);
	// Self-explanatory code
	float getVolume(void);

	//get the postion of the sound
	irrklang::vec3df getSoundPosition(void);
	//set the position of the sound
	void setSoundPosition(const float & x, const float & y, const float & z);
	void setSoundPosition(const irrklang::vec3df & newPos);


private:
	irrklang::ISound * _sound;
	SoundManager * soundManager;
	irrklang::vec3df _position;
	 std::string _filename;
	 bool _loop;
};
#endif
