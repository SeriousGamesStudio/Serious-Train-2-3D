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
	mKeyListeners.clear();
	mMouseListeners.clear();
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

void InputManager::addKeyListener(OIS::KeyListener * keyListener, const std::string & instanceName)
{
	if (_keyboard) {
		// Check for duplicate items
		itKeyListener = mKeyListeners.find(instanceName);
		if (itKeyListener == mKeyListeners.end()) {
			mKeyListeners[instanceName] = keyListener;
		}
		else {
			// Duplicate Item
		}
	}
}

void InputManager::addMouseListener(OIS::MouseListener * mouseListener, const std::string & instanceName)
{
	if (_mouse) {
		// Check for duplicate items
		itMouseListener = mMouseListeners.find(instanceName);
		if (itMouseListener == mMouseListeners.end()) {
			mMouseListeners[instanceName] = mouseListener;
		}
		else {
			// Duplicate Item
		}
	}
}

void InputManager::removeKeyListener(const std::string & instanceName)
{
	// Check if item exists
	itKeyListener = mKeyListeners.find(instanceName);
	if (itKeyListener != mKeyListeners.end()) {
		mKeyListeners.erase(itKeyListener);
	}
	else {
		// Doesn't Exist
	}
}

void InputManager::removeMouseListener(const std::string & instanceName)
{
	// Check if item exists
	itMouseListener = mMouseListeners.find(instanceName);
	if (itMouseListener != mMouseListeners.end()) {
		mMouseListeners.erase(itMouseListener);
	}
	else {
		// Doesn't Exist
	}
}

void InputManager::removeKeyListener(OIS::KeyListener * keyListener)
{
	itKeyListener = mKeyListeners.begin();
	itKeyListenerEnd = mKeyListeners.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
		if (itKeyListener->second == keyListener) {
			mKeyListeners.erase(itKeyListener);
			break;
		}
	}
}

void InputManager::removeMouseListener(OIS::MouseListener * mouseListener)
{
	itMouseListener = mMouseListeners.begin();
	itMouseListenerEnd = mMouseListeners.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) {
		if (itMouseListener->second == mouseListener) {
			mMouseListeners.erase(itMouseListener);
			break;
		}
	}
}

void InputManager::removeAllListeners(void)
{
	removeAllKeyListeners();
	removeAllMouseListeners();

}

void InputManager::removeAllKeyListeners(void)
{
	mKeyListeners.clear();
}

void InputManager::removeAllMouseListeners(void)
{
	mMouseListeners.clear();
}

OIS::Mouse * InputManager::getMouse(void)
{
	return _mouse;
}

OIS::Keyboard * InputManager::getKeyboard(void)
{
	return _keyboard;
}

void InputManager::update()
{
	_keyboard->capture();
	_mouse->capture();
}

bool InputManager::keyPressed(const OIS::KeyEvent & e)
{
	itKeyListener = mKeyListeners.begin();
	itKeyListenerEnd = mKeyListeners.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
		if (!itKeyListener->second->keyPressed(e))
			break;
	}

	return true;
}

bool InputManager::
keyReleased(const OIS::KeyEvent & e)
{
	itKeyListener = mKeyListeners.begin();
	itKeyListenerEnd = mKeyListeners.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
		if (!itKeyListener->second->keyReleased(e))
			break;
	}

	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent & e)
{
	itMouseListener = mMouseListeners.begin();
	itMouseListenerEnd = mMouseListeners.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) {
		if (!itMouseListener->second->mouseMoved(e))
			break;
	}

	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	itMouseListener = mMouseListeners.begin();
	itMouseListenerEnd = mMouseListeners.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) {
		if (!itMouseListener->second->mousePressed(e, id))
			break;
	}

	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	itMouseListener = mMouseListeners.begin();
	itMouseListenerEnd = mMouseListeners.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) {
		if (!itMouseListener->second->mouseReleased(e, id))
			break;
	}
	return true;
}
