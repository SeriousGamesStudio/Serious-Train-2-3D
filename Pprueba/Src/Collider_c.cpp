#include "Collider_c.h"
#include "Components.h"

Collider_c::Collider_c(btCollisionShape* shape) :
	Component(ComponentType::COLLIDER), _shape(shape)
{

}

Collider_c::~Collider_c()
{

}
