#include "EnemyFeedback_c.h"
#include "Notifications.h"
#include "Entity.h"


EnemyFeedback_c::EnemyFeedback_c(): Component(ComponentType::ENEMYFEEDBACK)
{
}

EnemyFeedback_c::~EnemyFeedback_c()
{
}

void EnemyFeedback_c::start()
{
	mesh_ = static_cast<MeshRenderer_c*>(_myEntity->getComponent(ComponentType::MESHRENDERER));
}

void EnemyFeedback_c::listen(Msg_Base * msg)
{
	switch (msg->id)
	{
	case MsgId::RAYCAST_HIT:

		Msg::Shoot* s = static_cast<Msg::Shoot*>(msg);
		if (s->collisionWith_) 
		{
			
			/*Cambiar material de la malla aqui desde la entidad de ogre del reciever */

		}

		break;
	default:
		break;
	}
}
