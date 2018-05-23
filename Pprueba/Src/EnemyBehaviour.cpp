#include "EnemyBehaviour.h"
#include "Scene.h"
#include "Collider_c.h"
#include "RigidBody_c.h"
#include "Walker_c.h"
#include "MeshRenderer_c.h"
#include <iostream>


EnemyBehaviour_c::EnemyBehaviour_c(Type t, bool frente) :
	Component(ComponentType::ENEMYBEHAVIOUR)
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
	wal->setVelocity(at.vel);
	_myEntity->getScene()->addListiner(MsgId::RAYCAST_HIT, this);
	if (dir > 0) //Coding God, pls forgive us
		static_cast<MeshRenderer_c*>(getComponent(ComponentType::MESHRENDERER))->rotate({ 0, 1, 0 }, 3.1416);
}

void EnemyBehaviour_c::update()
{
	//Me siento sucio :D
	float posZ = col->getCollisionObject().getWorldTransform().getOrigin().getZ();
	if ((dir < 0 && posZ <= 40.f) || (dir > 0 && posZ >= -20.f))
	{
		wal->setDirection(0, 0);
		//Aquí hay collision y sería guapo hacer que quitase vida o algo pero jeje no tiempo
	}
	else
		wal->setDirection(0, dir);
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
			if (at.hp <= 0) destroyMyEntity();
		}
		break;
	}
	default:
		break;
	}
}