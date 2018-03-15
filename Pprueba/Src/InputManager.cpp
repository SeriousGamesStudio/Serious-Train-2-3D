#include "InputManager.h"
#include "Game.h"



InputManager::InputManager(Game* game_)
{
	game = game_;
	_inputManager = NULL;
	_keyboard = NULL;
	_mouse = NULL;
	

	
}


InputManager::~InputManager()
{
	_inputManager->destroyInputObject(_keyboard);
	_keyboard = NULL;
	_inputManager->destroyInputObject(_mouse);
	_mouse = NULL;
	OIS::InputManager::destroyInputSystem(_inputManager);
	_inputManager = NULL;
}

bool InputManager::initInputManager() {
	Ogre::RenderWindow *window = game->getGraphicsManager().getWindow();
	size_t windowHandle = 0;
	window->getCustomAttribute("WINDOW", &windowHandle);
	
	
	std::string windowsHandleAsString = "";
	std::ostringstream windowHndStr;
	windowHndStr << windowHandle;
	windowsHandleAsString = windowHndStr.str();

	lSpecialParameters.insert(std::make_pair(std::string("WINDOW"), windowsHandleAsString));

	_inputManager = OIS::InputManager::createInputSystem(windowHandle);

	bool bufferKeys = false;
	bool bufferMouse = false;

	_keyboard  = static_cast<OIS::Keyboard*>(_inputManager->createInputObject(OIS::OISKeyboard, bufferKeys));

	_mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse, bufferMouse));

	const OIS::MouseState &infoSouris = _mouse->getMouseState();
	infoSouris.width = window->getWidth();
	infoSouris.height = window->getHeight();
	return true;
}

void InputManager::update()
{
	_keyboard->capture();
	_mouse->capture();
}
