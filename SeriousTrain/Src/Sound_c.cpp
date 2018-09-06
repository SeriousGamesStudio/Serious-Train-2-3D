#include "Sound_c.h"
#include "Components.h"
Sound_c::Sound_c(const std::string & file, const bool & loop): Sound(file, irrklang::vec3df(0.f, 0.f, 0.f),loop), Component(ComponentType::SOUND)
{
}

Sound_c::~Sound_c()
{
}

void Sound_c::start()
{
	playSound();
}

void Sound_c::update()
{
}

void Sound_c::listen(Msg_Base * msg)
{
	switch (msg->id)
	{
	case MsgId::CHANGED_POSITION:
	{
		Msg::ChangePosition* p = static_cast<Msg::ChangePosition*>(msg);
		setSoundPosition(p->x, p->y, p->z);
		break;
	}
	default:
		break;
	}
}
