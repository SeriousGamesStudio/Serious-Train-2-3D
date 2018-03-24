#include "Transform_c.h"
#include "Components.h"


Trasform_c::Trasform_c(Entity* entity, PhysicsManager* physicsManager, ConstructorInfo info) :
	t(btTransform(info.q, info.v)), Component(entity, ComponentType::TRANSFORM), _pM(physicsManager)
{}

Trasform_c::~Trasform_c()
{

}

void Trasform_c::start()
{

}
