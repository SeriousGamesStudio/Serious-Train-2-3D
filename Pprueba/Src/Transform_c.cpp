#include "Transform_c.h"



Trasform_c::Trasform_c(Entity* entity, PhysicsManager* physicsManager, ConstructorInfo info) :
	t(new btTransform(info.v, info.q)), Component(entity, "Transform"), _pM(physicsManager)
{}

Trasform_c::~Trasform_c()
{

}

void Trasform_c::start()
{

}
