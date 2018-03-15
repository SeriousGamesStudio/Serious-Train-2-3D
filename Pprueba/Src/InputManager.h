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

	void addKeyListener(OIS::KeyListener *keyListener, const std::string& instanceName);
	void addMouseListener(OIS::MouseListener *mouseListener, const std::string& instanceName);

	void removeKeyListener(const std::string& instanceName);
	void removeMouseListener(const std::string& instanceName);

	void removeKeyListener(OIS::KeyListener *keyListener);
	void removeMouseListener(OIS::MouseListener *mouseListener);

	void removeAllListeners(void);
	void removeAllKeyListeners(void);
	void removeAllMouseListeners(void);

	OIS::Mouse*    getMouse(void);
	OIS::Keyboard* getKeyboard(void);



	void update();
private:
	Game * game;
	OIS::InputManager * _inputManager;
	OIS::Mouse * _mouse;
	OIS::Keyboard * _keyboard;
	OIS::ParamList lSpecialParameters;

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	std::map<std::string, OIS::KeyListener*> mKeyListeners;
	std::map<std::string, OIS::MouseListener*> mMouseListeners;

	std::map<std::string, OIS::KeyListener*>::iterator itKeyListener;
	std::map<std::string, OIS::MouseListener*>::iterator itMouseListener;

	std::map<std::string, OIS::KeyListener*>::iterator itKeyListenerEnd;
	std::map<std::string, OIS::MouseListener*>::iterator itMouseListenerEnd;



};
#endif //!_H_INPUTMANAGER_H_

