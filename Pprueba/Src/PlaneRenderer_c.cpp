#include "PlaneRenderer_c.h"
#include "Components.h"
#include "GraphicsManager.h"

PlaneRenderer_c::PlaneRenderer_c(std::string meshName, std::string textName):
	Component(ComponentType::PLANERENDERER, "PlaneRenderer")
{
	plane_ = GraphicsManager::getInstance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("nPlane");

	Ogre::MeshPtr plane = Ogre::MeshManager::getSingleton().createPlane(meshName,
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, -25),
		(Ogre::Real)plane_->getCreator()->getCamera("MainCam")->getViewport()->getActualWidth(),
		(Ogre::Real)plane_->getCreator()->getCamera("MainCam")->getViewport()->getActualHeight(),
		10, 10, true, 1, 1.0, 1.0, Ogre::Vector3::UNIT_Z);
	
	Ogre::Entity* ent;
	ent = plane_->getCreator()->createEntity("entPlane", meshName);
	plane_->attachObject(ent);

	ent->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->createTextureUnitState(textName);

}

PlaneRenderer_c::~PlaneRenderer_c()
{
}