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

};


#include "Collider_c.h"
#include "RigidBody_c.h"
#include "Transform_c.h"

#include "MeshRenderer_c.h"

#include "Camera_c.h"
#include "PlayerController_c.h"
#include "CameraController_c.h"
#include "Walker_c.h"
#include <iostream>
#endif //!_H_COMPONENTS_H_

