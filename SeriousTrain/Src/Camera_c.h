#ifndef _H_CAMERA_C_H_
#define _H_CAMERA_C_H_

#include "Component.h"
#include "Components.h"
#include <Ogre.h>

class Camera_c :
	public Component
{
public:
	Camera_c();
	~Camera_c();
	
	virtual void update();

	void setPosition(float x, float y, float z);
	void rotateCamera(float x, float y, float z = 0.0f);

private:
	Ogre::Camera* cam;
	bool foreward, backward, left, right;
};
#endif // !_H_CAMERA_C_H_
