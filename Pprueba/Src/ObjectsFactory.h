#ifndef _H_OBJECTSFACTORY_H_
#define _H_OBJECTSFACTORY_H_

#include "Components.h"
#include <vector>
namespace ComponentConstructors { class ComponentConstructor; }
class Component;
class Entity;
class ComponentConstructorData;
class EntityConstructionData;
class Scene;

class ObjectsFactory
{
public:
	~ObjectsFactory();
	static ObjectsFactory* getInstance() 
	{
		if (!instance)
			instance = new ObjectsFactory();
		return instance;
	}
	//Creates an build an entity with info given
	 Entity* create(const EntityConstructionData& entityData, Scene* currentScene);

private:
	ObjectsFactory();
	/////////////////////BUILDERS/////////////////////////
	void buildEntity(Entity* e, const std::vector<ComponentConstructorData>& data);
	Component* buildComponent(ComponentType componentType, ComponentConstructors::ComponentConstructor* info);
	/////////////AUXILIAR FUNCTIONS///////////////////////
	unsigned int getUniqueId();
	//////////////////////////////////////////////////////
	unsigned int currentId;
	static ObjectsFactory* instance;
};

#endif //!_H_OBJECTSFACTORY_H_

