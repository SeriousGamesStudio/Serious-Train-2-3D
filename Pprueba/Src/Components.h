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
	ANIMATION,
	SOUND,
	SOUNDLISTENER,
	GAMEMANAGER,
	WEAPON,
	ENEMYBEHAVIOUR,
	SKYRENDERER,
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
	{ComponentType::WALKER,				"Walker"},
	{ComponentType::GAMEMANAGER,		"GameManager"},
	{ComponentType::ENEMYBEHAVIOUR,		"EnemyBehaiviour"},
	{ComponentType::SOUND,				"Sound"},
	{ComponentType::SOUNDLISTENER,		"SoundListener"},
	{ComponentType::WEAPON,				"Weapon"},
	{ComponentType::ANIMATION,			"Animation"},
	{ComponentType::SKYRENDERER,		"SkyRenderer"}
};


#endif //!_H_COMPONENTS_H_

