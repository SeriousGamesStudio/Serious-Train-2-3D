#include "MeshRenderer_c.h"
#include "GraphicsManager.h"
#include "Components.h"

MeshRenderer_c::MeshRenderer_c(std::string meshName) :
	Component(ComponentType::MESHRENDERER)
{
	scnNode_ = GraphicsManager::getInstance()->createNewNode(meshName);
}

MeshRenderer_c::~MeshRenderer_c()
{
}

void MeshRenderer_c::listen(Msg_Base* msg)
{
	switch (msg->id)
	{
	default:
		break;
	case MsgId::CHANGED_POSITION:
		Msg::ChangePosition* p = static_cast<Msg::ChangePosition*>(msg);
		float px = p->x;
		float py = p->y;
		float pz = p->z;
		//printf("Robot position: (%f, %f, %f)\n", px, py, pz);
		scnNode_->setPosition(p->x, p->y, p->z);
		break;
	}
}
