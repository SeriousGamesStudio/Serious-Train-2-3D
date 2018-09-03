#ifndef _H_PLANERENDERER_H_
#define  _H_PLANERENDERER_H_
#include "Component.h"
#include "Components.h"

#include <string>
#include <Ogre.h>
class GraphicsManager;

class PlaneRenderer_c:
	public Component
{
public:
	PlaneRenderer_c(std::string meshName, std::string textName, int level);
	~PlaneRenderer_c();

private:
	Ogre::SceneNode* plane_;
	Ogre::MeshPtr plane;
	std::string meshName_;
	int level_;
	Ogre::Entity* ent;
};

#endif //!_H_PLANERENDERER_H_

