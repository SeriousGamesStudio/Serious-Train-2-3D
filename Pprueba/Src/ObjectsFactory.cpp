#include "ObjectsFactory.h"
#include "ComponentsConstructors.h"
#include "SceneManager.h"
#include "DataManager.h"    // for EntityConstructionData
#include "Entity.h"         // for Entity
#include "Component.h"

 ObjectsFactory* ObjectsFactory::instance = nullptr;
 std::string ObjectsFactory::stringIdOfPrefab[ObjectsFactory::Prefab::size];

ObjectsFactory::ObjectsFactory():
	currentId(0)
{
	instance = this;
	init();
}
void ObjectsFactory::init()
{
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
	Component* newComponent = nullptr;

	switch (componentType)
	{
	case ComponentType::RIGIDBODY:
		{
			//PEREZA! hay que hacer el constructor del rigid body y todo eso -.-''
		}	
		break;
	case ComponentType::TRANSFORM: 
		{
		auto* c = static_cast<ComponentConstructors::Transform*>(info);
		// este transform está obsoleto, así que paso
		//newComponent = new Trasform_c();
		}
		break;
	case ComponentType::COLLIDER: 
		{
		//Componente obsoleto, paso
		}
		break;
	case ComponentType::MESHRENDERER: 
		{
			auto* c = static_cast<ComponentConstructors::MeshRenderer*>(info);
			newComponent = new MeshRenderer_c(c->meshPath);
		}
		break;
	case ComponentType::PLAYERCONTROLLER: 
		{
			newComponent = new PlayerController_c();
		}
		break;
	case ComponentType::CAMERACONTROLLER: 
		{
			newComponent = new CameraController_c();
		}
		break;
	case ComponentType::CAMERA: 
		{
			newComponent = new Camera_c();
		}
		break;
	case ComponentType::PLANERENDERER: 
		{
			auto* c = static_cast<ComponentConstructors::PlaneRenderer*>(info);
			newComponent = new PlaneRenderer_c(c->meshPath, c->texturePath);
		}
		break;
	case ComponentType::WALKER: 
		{
			newComponent = new Walker_c();
		}
		break;
	default: break;
	}

	return newComponent;
}

unsigned int ObjectsFactory::getUniqueId(){	return ++currentId; }

