#ifndef _H_MESHRENDERER_H_
#define _H_MESHRENDERER_H_
#include "Component.h"
#include "Ogre.h"

using namespace Ogre;
class MeshRenderer_c:
	public Component
{
public:
	MeshRenderer_c(m_Entity* m_entity, Ogre::Entity* myEntity);
	~MeshRenderer_c();

	void drawPlane(SceneNode* scnNode);

};
#endif // !_H_MESHRENDERER_H_

