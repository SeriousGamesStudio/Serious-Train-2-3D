#ifndef _H_OBJECTSFACTORY_H_
#define _H_OBJECTSFACTORY_H_

#include "Components.h"
namespace ComponentConstructors { class ComponentConstructor; }
struct EntityConstructionData;
typedef std::map<ComponentType, ComponentConstructors::ComponentConstructor*> ConstructionData;
class Component;
class Entity;

class ObjectsFactory
{
	enum Prefab 
	{
		CUSTOM,
		PLAYER,
		ENEMY,
		size
	};


public:
	ObjectsFactory();
	~ObjectsFactory();

	static Entity* create(EntityConstructionData& entityData);

private:
	/////////////////////BUILDERS/////////////////////////
	void buildEntity(Prefab prefab, Entity* e, ConstructionData& data);
	Component* buildComponent(ComponentType componentType, ComponentConstructors::ComponentConstructor* info);
	/////////////AUXILIAR FUNCTIONS///////////////////////
	unsigned int getUniqueId();
	//////////////////////////////////////////////////////
	unsigned int currentId;
	static ObjectsFactory* instance;
	std::vector<ComponentType> componentsOfPrefab[Prefab::size];
};

#endif //!_H_OBJECTSFACTORY_H_

