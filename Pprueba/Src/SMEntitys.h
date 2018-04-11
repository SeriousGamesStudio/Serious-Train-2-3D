#ifndef _SCENE_MAKER_ENTITYS_
#define _SCENE_MAKER_ENTITYS_

#include <vector>
#include <string>
#include "SMComponents.h"
#include "SMstringOperations.h"

static EntityFactoy* instance = nullptr;
class EntityFactoy
{
public:
	enum Prefab
	{
		PLAYER,
	};
	static EntityFactoy* get() 
	{
		if (!instance)
			instance = new EntityFactoy();
		return instance;
	};
	~EntityFactoy() { instance = nullptr; };

	std::vector<SceneMaker::Components::Component*> create(Prefab prefab)
	{
		using namespace SceneMaker::Components;
		std::vector<Component*> entity;
		for (auto component : prefabs[prefab])
			entity.push_back(factory(component));
		return entity;
	};
private:
	EntityFactoy() {};
	using CT =  SceneMaker::Components::Type;
	const std::vector<std::vector<CT>> prefabs =
	{
		//Player
		{
			CT::CAMERA,
			CT::CAMERA_CONTROLLER,
			CT::MESH_RENDERER,
			CT::WALKER,
			CT::RIGIDBODY
		}
	};

};

#endif // !_SCENE_MAKER_ENTITYS_

