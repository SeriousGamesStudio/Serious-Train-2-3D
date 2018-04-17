#include "Transform_c.h"
#include "Components.h"


Transform_c::Transform_c(const btVector3& position, const btQuaternion& rotation):
	Component(ComponentType::TRANSFORM), t(rotation, position)
{}

Transform_c::~Transform_c()
{

}

void Transform_c::start()
{

}
