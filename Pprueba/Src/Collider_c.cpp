#include "Collider_c.h"
#include "Components.h"

Collider_c::Collider_c(btCollisionShape* shape) :
	Component(ComponentType::COLLIDER,"Collider"), _shape(shape)
{

}

Collider_c::~Collider_c()
{

}
