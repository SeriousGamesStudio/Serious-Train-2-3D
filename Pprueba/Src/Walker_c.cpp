#include "Entity.h"
#include "Components.h"

Walker_c::Walker_c():
	Component(ComponentType::CAMERA)
{
	
	rb = nullptr;
	
}

Walker_c::~Walker_c()
{

}

void Walker_c::start() {
	rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
}
void Walker_c::update()
{
	//Esto es provisional hasta que se implemente el resolveDependences.
	if (!rb) rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));

	
}

void Walker_c::listen(Msg_Base * msg)
{
	switch (msg->id)		//tipo de mensaje
	{
		case MsgId::LOOKING_TO:
			{
			Msg::LookAt * l = static_cast<Msg::LookAt*>(msg);
			dirX = l->x;
			dirZ = l->z;
			break;
			}
		case MsgId::PLAYER_MOVE:
		{
			Msg::Move * p = static_cast<Msg::Move*>(msg);
			switch (p->info)		//info del tipo del mensaje
			{
			case Msg::Move::Dir::FOREWARD:
				std::cout << "caminando";
				rb->get()->applyCentralImpulse(btVector3(0, 0, 1));
				break;
			case Msg::Move::Dir::BACKWARD:
				rb->get()->applyCentralImpulse(btVector3(0, 0, -1));
				break;
			case Msg::Move::Dir::LEFT:
				rb->get()->applyCentralImpulse(btVector3(-1, 0, 0));
				break;
			case Msg::Move::Dir::RIGHT:
				rb->get()->applyCentralImpulse(btVector3(1, 0, 0));
				break;
			default:
				break;
			}
			break;
		}
	default:
		break;
	}

}