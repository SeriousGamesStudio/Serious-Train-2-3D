#include "MeshRenderer_c.h"

MeshRenderer_c::MeshRenderer_c(ConstructorInfo info, m_Entity* m_entity, SceneNode* scnNode): Component(m_entity)
{
	
		switch (info.type)
		{
		case PLANE:
			mesh_ = MeshManager::getSingleton().createPlane("mFondo",
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Plane(Vector3::UNIT_Y, -25),
				(Real)scnNode->getCreator()->getCamera("MainCam")->getViewport()->getActualWidth(),
				(Real)scnNode->getCreator()->getCamera("MainCam")->getViewport()->getActualHeight(),
				10, 10, true, 1, 1.0, 1.0, Vector3::UNIT_Z);
			Ogre::Entity* myPlane = scnNode->getCreator()->createEntity("entFondo", "mFondo");
			scnNode->attachObject(myPlane);
			break;
		}
	
}

MeshRenderer_c::~MeshRenderer_c()
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
