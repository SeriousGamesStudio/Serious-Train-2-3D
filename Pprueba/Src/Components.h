#ifndef _H_COMPONENTS_H_
#define _H_COMPONENTS_H_

enum ComponentType
{
	RIGIDBODY,
	TRANSFORM,
	COLLIDER,
	MESHRENDERER,
	PLAYERCONTROLLER,
	CAMERACONTROLLER,
	CAMERA,
	PLANERENDERER,
	WALKER,
	size//This element MUST be always the last one

};
#include <string>
static std::string getComponentStringId[ComponentType::size];

#include "Collider_c.h"
#include "RigidBody_c.h"
#include "Transform_c.h"

#include "MeshRenderer_c.h"

#include "Camera_c.h"
#include "PlayerController_c.h"
#include "CameraController_c.h"
#include "Walker_c.h"
#include "PlaneRenderer_c.h"
#endif //!_H_COMPONENTS_H_

