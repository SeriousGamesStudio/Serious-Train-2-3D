#include "Transform_c.h"
#include "Components.h"


Trasform_c::Trasform_c(ConstructorInfo info) :
	t(btTransform(info.q, info.v)), Component(ComponentType::TRANSFORM, "Transform")
{}

Trasform_c::~Trasform_c()
{

}

void Trasform_c::start()
{

}
