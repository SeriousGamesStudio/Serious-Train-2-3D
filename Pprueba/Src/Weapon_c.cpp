#include "Weapon_c.h"
#include "Components.h"
#include "Entity.h"

Weapon_c::Weapon_c():
	Component(ComponentType::WEAPON)
{
	InputManager::getInstance()->addMouseListener(this, "ratonJugador");
}

Weapon_c::~Weapon_c()
{
}
void Weapon_c::start()
{
	 rb_ = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
}

void Weapon_c::update()
{
}

bool Weapon_c::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	//raycast
	/*btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	
	// Perform raycast
	
	World->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit()) {
		End = RayCallback.m_hitPointWorld;
		Normal = RayCallback.m_hitNormalWorld;

		// Do some clever stuff here
	}
	*/
	return false;
}

bool Weapon_c::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
	return false;
}


void Weapon_c::listen(Msg_Base * msg)
{
}
