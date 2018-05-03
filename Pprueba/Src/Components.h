#ifndef _H_COMPONENTS_H_
#define _H_COMPONENTS_H_

#include "Collider_c.h"
#include "RigidBody_c.h"
#include "Transform_c.h"

#include "MeshRenderer_c.h"

#include "Camera_c.h"
#include "PlayerController_c.h"
#include "CameraController_c.h"
#include "Walker_c.h"
#include "PlaneRenderer_c.h"
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
	ANIMATION,
	SOUND,
	SOUNDLISTENER,
	GAMEMANAGER,
	WEAPON,
	ENEMYBEHAVIOUR,
	size

};
#include <string>
#include <map>

static std::map<ComponentType, const std::string> getComponentString = 
{
	{ComponentType::RIGIDBODY,			"RigidBody"},
	{ComponentType::TRANSFORM,			"Transform"},
	{ComponentType::COLLIDER,			"Collider"},
	{ComponentType::MESHRENDERER,		"MeshRenderer"},
	{ComponentType::PLAYERCONTROLLER,	"PlayerController"},
	{ComponentType::CAMERACONTROLLER,	"CameraController"},
	{ComponentType::CAMERA,				"Camera"},
	{ComponentType::PLANERENDERER,		"PlaneRenderer"},
	{ComponentType::WALKER,				"Walker"}
};

#include "Camera_c.h"
#include "PlayerController_c.h"
#include "CameraController_c.h"
#include "Animation_c.h"
#include "Walker_c.h"
#include "PlaneRenderer_c.h"
#include "Sound_c.h"
#include "SoundListener_c.h"
#include "GameManager_c.h"
#include "Weapon_c.h"
#include "EnemyBehaviour.h"
#endif //!_H_COMPONENTS_H_

