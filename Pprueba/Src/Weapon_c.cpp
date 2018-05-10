#include "Components.h"
#include "Weapon_c.h"
#include "Transform_c.h"
#include "Entity.h"
#include "Notifications.h"
#include "PhysicsManager.h"
#include <iostream>
Weapon_c::Weapon_c(float r, int dmg, float cad):
	Component(ComponentType::WEAPON), range_(r), dmg_(dmg), cadency_(cad)
{
	InputManager::getInstance()->addMouseListener(this, "ratonJugador");
	aiming = btVector3(0, 0, 0); // no disparar si no te has movido pls
}

Weapon_c::~Weapon_c()
{
}
void Weapon_c::start()
{
	 t = static_cast<Transform_c*>(_myEntity->getComponent(ComponentType::TRANSFORM));
}

void Weapon_c::update()
{
}

bool Weapon_c::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	//raycast
	btVector3 origin = t->get().getOrigin();

	btVector3 end = t->get().getOrigin() + (aiming*range_);
	btCollisionWorld::ClosestRayResultCallback RayCallback(t->get().getOrigin(), end);
	
	// Perform raycast
	
	PhysicsManager::getInstance()->getWorld()->rayTest(t->get().getOrigin(), end, RayCallback);

	if (RayCallback.hasHit()) {
		end = RayCallback.m_hitPointWorld;
		//btVector3 normal = RayCallback.m_hitNormalWorld;
		std::cout << "disparo"<< std::endl;
		sendMsg(new Msg::Shoot(_myEntity->getId(), Msg_Base::broadcast, dmg_, (void*)RayCallback.m_collisionObject));
		// Do some clever stuff here skrr
	}
	
	return false;
}

bool Weapon_c::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return false;
}


void Weapon_c::listen(Msg_Base * msg)
{
	switch (msg->id)
	{
	case MsgId::LOOKING_TO: 
	{
		Msg::LookAt* p = static_cast<Msg::LookAt*>(msg);
		aiming.setValue(p->x, p->y, p->z);
	}

		break;
	default:
		break;
	}
}
