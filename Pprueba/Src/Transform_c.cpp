#include "Transform_c.h"
#include "Components.h"
#include "Entity.h"

Transform_c::Transform_c(const btVector3& position, const btQuaternion& rotation):
	Component(ComponentType::TRANSFORM), t(rotation, position), old(rotation, position)
{}

Transform_c::Transform_c(float px, float py, float pz, float qx, float qy, float qz, float qw):
	Component(ComponentType::TRANSFORM), t(btQuaternion(qx, qy, qz, qw), btVector3(px, py, pz))
{
}
void Transform_c::start() {
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
Transform_c::~Transform_c(){}