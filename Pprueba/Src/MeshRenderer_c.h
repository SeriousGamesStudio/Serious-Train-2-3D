#ifndef _H_MESHRENDERER_H_
#define _H_MESHRENDERER_H_
#include "Component.h"
#include "Components.h"

#include <string>
#include <Ogre.h>
class GraphicsManager;


class MeshRenderer_c :
	public Component
{
public:
	MeshRenderer_c(std::string meshName, float scale);
	~MeshRenderer_c();
	
	virtual void listen(Msg_Base* msg);
	inline Ogre::SceneNode* getSceneNode() { return scnNode_; }
	

private:
	Ogre::SceneNode* scnNode_;

};
#endif // !_H_MESHRENDERER_H_

