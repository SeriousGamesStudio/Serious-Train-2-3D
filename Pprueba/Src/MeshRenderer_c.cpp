#include "MeshRenderer_c.h"
#include "GraphicsManager.h"

MeshRenderer_c::MeshRenderer_c(Entity* m_entity, GraphicsManager* gphMgr, std::string meshName) :
	Component(m_entity, "MeshRenderer")
{

	scnNode_ = gphMgr->createNewNode(meshName);


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
