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