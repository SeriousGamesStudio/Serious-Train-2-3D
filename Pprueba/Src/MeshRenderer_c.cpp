#include "MeshRenderer_c.h"
#include "GraphicsManager.h"

MeshRenderer_c::MeshRenderer_c(std::string meshName, float scale) :
	Component(ComponentType::MESHRENDERER)
{
	scnNode_ = GraphicsManager::getInstance()->createNewNode(meshName);
	scnNode_->scale(scale, scale, scale);
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
		scnNode_->setPosition(p->x, p->y, p->z);
		break;
	}
}
