#ifndef _H_MESHRENDERER_H_
#define _H_MESHRENDERER_H_
#include "Component.h"
#include <Ogre.h>

#include <string>
class GraphicsManager;


class MeshRenderer_c :
	public Component
{
public:
	MeshRenderer_c(std::string meshName);
	~MeshRenderer_c();

	virtual void listen(Msg_Base* msg);


private:
	Ogre::SceneNode* scnNode_;

};
#endif // !_H_MESHRENDERER_H_

