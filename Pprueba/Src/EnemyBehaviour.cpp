#include "EnemyBehaviour.h"
#include "Scene.h"
#include "Collider_c.h"
#include "RigidBody_c.h"
#include "Walker_c.h"
#include "SceneManager.h"
#include <iostream>


EnemyBehaviour_c::EnemyBehaviour_c(Type t, bool frente) :
	Component(ComponentType::ENEMYBEHAVIOUR), feedback_(false), counter(0), lifeCounter_(0)

{
	dir = (frente) ? 1 : -1;
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
	_myEntity->getScene()->removeListener(MsgId::RAYCAST_HIT, this);
}
void EnemyBehaviour_c::start()
{
	col = static_cast<Collider_c*>(_myEntity->getComponent(ComponentType::COLLIDER));
	
	wal = static_cast<Walker_c*>(_myEntity->getComponent(ComponentType::WALKER));
	if (!wal)
			std::cout<< "no se ha encontrado walker"<< std::endl;
	_myEntity->getScene()->addListiner(MsgId::RAYCAST_HIT, this);
	
}

void EnemyBehaviour_c::update()
{
	
	if (col->getCollisionObject().getWorldTransform().getOrigin().getZ() <= 50 &&
		col->getCollisionObject().getWorldTransform().getOrigin().getZ() >= -28) 
	{
		wal->setDirection(0, 0);
		if(lifeCounter_ >= 30)
		{
			sendMsg(new Msg::DamageTrain(_myEntity->getId(), Msg_Base::broadcast, at.dmg));
			lifeCounter_ = 0;
		}
		else lifeCounter_++;

	}
	else
		wal->setDirection(0, at.vel * dir);

	if (counter >= 20)
	{
		counter = 0;
		feedback_ = false;		
		sendMsg(new Msg::TextureReset(_myEntity->getId(), Msg_Base::self));
	}
	else
		counter++;


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
			if (at.hp <= 0) {
				destroyMyEntity(); // destroy entity 
				SceneManager::getInstance()->currentScene()->restaEnemigo();
			}
			else
			{
				sendMsg(new Msg::EnemyFeedback(_myEntity->getId(), Msg_Base::self));
				feedback_ = true;
			}
		}
		
		break;
	}
	default:
		break;
	}
}