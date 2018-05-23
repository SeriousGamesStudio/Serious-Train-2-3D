#include "MeshRenderer_c.h"
#include "GraphicsManager.h"
#include "Entity.h"
#include "Transform_c.h"
MeshRenderer_c::MeshRenderer_c(std::string meshName, float scale) :
	Component(ComponentType::MESHRENDERER)
{
	meshPath = meshName;
	this->scale = scale;
}

MeshRenderer_c::~MeshRenderer_c()
{
	GraphicsManager::getInstance()->getSceneManager()->destroySceneNode(scnNode_);
}
void MeshRenderer_c::start()
{
	scnNode_ = GraphicsManager::getInstance()->createNewNode(meshPath, _myEntity->getName());
	scnNode_->scale(scale, scale, scale);
	static_cast<Transform_c*>(getComponent(ComponentType::TRANSFORM))->get().getRotation();
	//Hay que rotar el nodo  con el transform que hemos cogido

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
