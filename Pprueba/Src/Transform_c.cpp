#include "Transform_c.h"
#include "Components.h"


Trasform_c::Trasform_c(Entity* entity, ConstructorInfo info) :
	t(btTransform(info.q, info.v)), Component(entity, ComponentType::TRANSFORM)
{}

Trasform_c::~Trasform_c()
{

}

void Trasform_c::start()
{

}
