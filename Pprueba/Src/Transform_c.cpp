#include "Transform_c.h"
#include "Components.h"
#include "Entity.h"

Transform_c::Transform_c(const btVector3& position, const btQuaternion& rotation):
	Component(ComponentType::TRANSFORM), t(rotation, position), old(rotation, position)
{}

Transform_c::~Transform_c()
{

}

void Transform_c::start()
{
	sendMsg(new Msg::ChangePosition(_myEntity->getId(), Msg_Base::self, t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ()));
}

void Transform_c::update()
{
	if (t.getOrigin() != old.getOrigin())
	{
		old.setOrigin(t.getOrigin());
		sendMsg(new Msg::ChangePosition(_myEntity->getId(), Msg_Base::self, t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ()));
	}
}
void Transform_c::setPosition(float x, float y, float z)
{
	t.getOrigin().setValue(x, y, z);
	sendMsg(new Msg::ChangePosition(_myEntity->getId(), Msg_Base::self, x, y, z));
}
