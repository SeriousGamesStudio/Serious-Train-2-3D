#include "Components.h"
#include "Entity.h"
#include "Notifications.h"


EnemyBehaviour_c::EnemyBehaviour_c(Type t) :
	Component(ComponentType::ENEMYBEHAVIOUR)
{
	switch (t)
	{
	case EnemyBehaviour_c::NORMAL:
		at = normal;
		break;
	case EnemyBehaviour_c::FLY:
		at = fly2fly;
		break;
	default:
		break;
	}
}

EnemyBehaviour_c::~EnemyBehaviour_c()
{
}
void EnemyBehaviour_c::start()
{
	col = static_cast<Collider_c*>(_myEntity->getComponent(ComponentType::COLLIDER));
	rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
}

void EnemyBehaviour_c::update()
{
}



void EnemyBehaviour_c::listen(Msg_Base * msg)
{
	switch (msg->id)
	{
	case MsgId::RAYCAST_HIT:
	{
		Msg::Shoot* p = static_cast<Msg::Shoot*>(msg);
		if ((btCollisionObject*)p->collisionWith_ == &col->getCollisionObject())
		{
			at.hp -= p->dmg_;
			if (at.hp <= 0); // destroy entity ajjaj
			//feedback del raycast
			rb->get()->applyForce(btVector3(0, 5, 0), rb->get()->getCenterOfMassPosition());
		}


		break;
	}
	default:
		break;
	}
}