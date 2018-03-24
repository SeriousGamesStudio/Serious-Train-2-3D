#include "Collider_c.h"
#include "Components.h"

Collider_c::Collider_c(Entity* entity, PhysicsManager* physicsManager, btCollisionShape* shape) :
	Component(entity, ComponentType::COLLIDER), _pM(physicsManager), _shape(shape)
{

}

Collider_c::~Collider_c()
{

}
