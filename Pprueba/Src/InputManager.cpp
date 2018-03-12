#include "InputManager.h"
#include "Game.h"



InputManager::InputManager(Game* game)
{

}

InputManager::~InputManager()
{

}

void InputManager::update()
{

}


void InputManager::subscribe(INPUT_NOTIFICATION notification, std::function<void> callback)
{
	if (!subscribers.count(notification))
		subscribers.emplace(notification);

	subscribers[notification].push_back(callback);
}

void InputManager::unsubscribe(INPUT_NOTIFICATION notification, std::function<void> callback)
{
}

void InputManager::parseEvents()
{
	//Traducir los eventos del sistema a los que a nosotros nos interesan
}

void InputManager::dispachNotificacations()
{
	//hacer las llamadas a los callbacks pertinentes
}
