#ifndef _H_MESHRENDERER_H_
#define _H_MESHRENDERER_H_
#include "Component.h"
#include "Ogre.h"
#include "GraphicsManager.h"
#include <string>


class MeshRenderer_c:
	public Component
{
public:
	MeshRenderer_c(m_Entity* m_entity, GraphicsManager* gphMgr, std::string meshName);
	~MeshRenderer_c();

	virtual void start();
 

private:
	Ogre::SceneNode* scnNode_;
		
};
#endif // !_H_MESHRENDERER_H_

