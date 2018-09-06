#include "PlaneRenderer_c.h"
#include "GraphicsManager.h"

PlaneRenderer_c::PlaneRenderer_c(std::string meshName, std::string textName, int level):
	Component(ComponentType::PLANERENDERER), meshName_(meshName), level_(level)
{
	plane_ = GraphicsManager::getInstance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("nPlane" + std::to_string(level));


		plane = Ogre::MeshManager::getSingleton().createPlane(meshName + std::to_string(level),
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, -25),
		(Ogre::Real)plane_->getCreator()->getCamera("MainCam")->getViewport()->getActualWidth(),
		(Ogre::Real)plane_->getCreator()->getCamera("MainCam")->getViewport()->getActualHeight(),
		100, 100, true, 1, 1.0, 1.0, Ogre::Vector3::UNIT_Z);
	
	
	ent = plane_->getCreator()->createEntity("entPlane" + std::to_string(level), meshName + std::to_string(level));
	plane_->attachObject(ent);

	ent->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->createTextureUnitState(textName);

}

PlaneRenderer_c::~PlaneRenderer_c()
{
	Ogre::deletePtr(plane_);
	Ogre::deletePtr(ent);
	//plane->destroySubMesh(meshName_ + std::to_string(level_));
	
	
}