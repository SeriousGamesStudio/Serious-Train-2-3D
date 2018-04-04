#include "Collider_c.h"
#include "Components.h"

Collider_c::Collider_c(Entity* entity, btCollisionShape* shape) :
	Component(entity, ComponentType::COLLIDER), _shape(shape)
{

}

Collider_c::~Collider_c()
{

}
