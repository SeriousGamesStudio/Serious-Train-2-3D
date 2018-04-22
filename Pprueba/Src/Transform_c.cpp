#include "Transform_c.h"
#include "Components.h"


Transform_c::Transform_c(const btVector3& position, const btQuaternion& rotation):
	Component(ComponentType::TRANSFORM), t(rotation, position)
{}

Transform_c::Transform_c(float px, float py, float pz, float qx, float qy, float qz, float qw):
	Component(ComponentType::TRANSFORM), t(btQuaternion(qx, qy, qz, qw), btVector3(px, py, pz))
{

}
Transform_c::~Transform_c(){}