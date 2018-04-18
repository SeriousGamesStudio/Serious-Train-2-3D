#include "Component.h"
#include "Components.h"
#include "Entity.h"
#include "Scene.h"
#include "Notifications.h"

#include <string>

Component::Component(ComponentType type_, std::string typeString) :
	_enable(true), type(type_)
{
	getComponentStringId[type] = typeString;
}

Component::~Component()
{

}

void Component::sendMsg(Msg_Base* newMsg)
{
	if (newMsg->reciver == Msg_Base::self)
		_myEntity->reciveMsg(newMsg);
	else
		_myEntity->getScene()->reciveMsg(newMsg);


}