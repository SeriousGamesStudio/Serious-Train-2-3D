#ifndef _H_COMPONENTS_H_
#define _H_COMPONENTS_H_

enum class ComponentType
{
	RIGIDBODY,
	TRANSFORM,
	COLLIDER,
	MESHRENDERER,
	PLAYERCONTROLLER,
	CAMERACONTROLLER,
	CAMERA,
	ANIMATION,

};


#include "Collider_c.h"
#include "RigidBody_c.h"
#include "Transform_c.h"

#include "MeshRenderer_c.h"

#include "Camera_c.h"
#include "PlayerController.h"
#include "CameraController_c.h"
#include "Animation_c.h"
#endif //!_H_COMPONENTS_H_

