#ifndef _H_CAMERA_C_H_
#define _H_CAMERA_C_H_

#include "Component.h"
#include "GraphicsManager.h"

#include "Entity.h"
#include "Components.h"
#include <Ogre.h>

class Camera_c :
	public Component
{
public:
	Camera_c(Entity * entity, GraphicsManager* graphicsManager);
	~Camera_c();
	
	virtual void update();
	virtual void listen(Msg_Base* msg);

	inline Ogre::Camera* getCamera() { return cam; }
private:
	GraphicsManager* graphicsManager_;
	Ogre::Camera * cam;
	Ogre::Viewport * vp;
	Ogre::SceneNode* camNode_;

	bool foreward, backward, left, right;
};
#endif // !_H_CAMERA_C_H_
