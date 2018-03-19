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

void MeshRenderer_c::start()
{

}

//void MeshRenderer_c::drawPlane(SceneNode * scnNode)
//{
//	MeshPtr plane = MeshManager::getSingleton().createPlane("mFondo",
//		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//		Plane(Vector3::UNIT_Y, -25),
//		(Real)scnNode->getCreator()->getCamera("MainCam")->getViewport()->getActualWidth(),
//		(Real)scnNode->getCreator()->getCamera("MainCam")->getViewport()->getActualHeight(),
//		10, 10, true, 1, 1.0, 1.0, Vector3::UNIT_Z);
//	
//	Ogre::Entity* myPlane = scnNode->getCreator()->createEntity("entFondo", "mFondo");
//	scnNode->attachObject(myPlane);
//	
//	//scnNode->attachObject(scnNode->getCreator()->createEntity("entFondo", "mFondo"))
//}
