#ifndef _H_INPUTMANAGER_H_
#define _H_INPUTMANAGER_H_
#include <OIS.h>

class Game;


class InputManager
{
public:
	InputManager(Game* game_);
	~InputManager();
	bool initInputManager();

	void update();
private:
	Game * game;
	OIS::InputManager * _inputManager;
	OIS::Mouse * _mouse;
	OIS::Keyboard * _keyboard;
	OIS::ParamList lSpecialParameters;

};
#endif //!_H_INPUTMANAGER_H_

