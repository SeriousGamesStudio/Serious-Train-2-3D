#include "ObjectsFactory.h"
#include "SceneManager.h"
#include "DataManager.h"    // for EntityConstructionData
#include "Entity.h"         // for Entity
#include "vcruntime_new.h"  // for operator new

static ObjectsFactory* instance = nullptr;

ObjectsFactory::ObjectsFactory():
	currentId(0)
{
	instance = this;
	{//Player init
		componentsOfPrefab[Prefab::PLAYER] =
		{
			ComponentType::CAMERA,
			ComponentType::CAMERACONTROLLER,
			ComponentType::COLLIDER,
			ComponentType::TRANSFORM,
			ComponentType::WALKER
		};
		stringIdOfPrefab[Prefab::PLAYER] = "Player"; 
	}
	{//Enemy init
		componentsOfPrefab[Prefab::ENEMY] = 
		{
			ComponentType::COLLIDER,
			ComponentType::TRANSFORM,
			ComponentType::WALKER
			//etc
		};
		stringIdOfPrefab[Prefab::ENEMY] = "Enemy";
	}

}

ObjectsFactory::~ObjectsFactory()
{
}

Entity * ObjectsFactory::create(EntityConstructionData & entityData)
{
	Entity* newEntity = new Entity(SceneManager::getInstance()->currentScene(), instance->getUniqueId(), entityData.entityName);

	instance->buildEntity((Prefab)((int)entityData.prefab), newEntity, entityData.data);

	return newEntity;
}

void ObjectsFactory::buildEntity(Prefab prefab, Entity * e, ConstructionData & data)
{
	for (ComponentType componentType : componentsOfPrefab[prefab])
	{
		e->addComponent(buildComponent(componentType, data[componentType]));
	}
	e->init();
}

Component * ObjectsFactory::buildComponent(ComponentType componentType, ComponentConstructors::ComponentConstructor* info)
{
	return nullptr;
}

unsigned int ObjectsFactory::getUniqueId()
{
	return ++currentId;
}
