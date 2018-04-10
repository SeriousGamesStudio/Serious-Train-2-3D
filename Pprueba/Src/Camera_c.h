#ifndef _H_CAMERA_C_H_
#define _H_CAMERA_C_H_

#include "Component.h"
#include <Ogre.h>
class GraphicsManager;


class Camera_c :
	public Component
{
public:
	Camera_c(GraphicsManager* graphicsManager);
	~Camera_c();
	
	virtual void update();
	virtual void listen(Msg_Base* msg);

	void setPosition(float x, float y, float z);

private:
	GraphicsManager* graphicsManager_;
	Ogre::Camera* cam;
	bool foreward, backward, left, right;
};
#endif // !_H_CAMERA_C_H_
