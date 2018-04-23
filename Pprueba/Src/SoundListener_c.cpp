#include "SoundManager.h"
#include "Components.h"
#include "Entity.h"
#include "SoundListener_c.h"

SoundListener_c::SoundListener_c(): Component(ComponentType::SOUNDLISTENER)
{
}

SoundListener_c::~SoundListener_c()
{
}

void SoundListener_c::start()
{
	SoundManager::getInstance()->setListenerPosition(lookAt, position);
}

void SoundListener_c::update()
{
}

void SoundListener_c::listen(Msg_Base * msg)
{
	switch (msg->id)
	{
	case MsgId::LOOKING_TO:
	{
		Msg::LookAt* look = static_cast<Msg::LookAt*>(msg);
		lookAt = Ogre::Vector3(look->x, look->y, look->z);
		SoundManager::getInstance()->setListenerPosition(lookAt, position);
		break;
	}
	case MsgId::CHANGED_POSITION:
	{
		Msg::ChangePosition* p = static_cast<Msg::ChangePosition*>(msg);
		position = Ogre::Vector3(p->x, p->y, p->z);
		SoundManager::getInstance()->setListenerPosition(lookAt, position);
		break;
	}
	default:
		break;
	}
}
