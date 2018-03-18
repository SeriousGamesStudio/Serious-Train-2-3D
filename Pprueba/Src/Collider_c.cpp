#include "Collider_c.h"



Collider_c::Collider_c(Entity* entity, PhysicsManager* physicsManager, btCollisionShape* shape) :
	Component(entity), _pM(physicsManager), _shape(shape)
{

}

Collider_c::~Collider_c()
{

}
