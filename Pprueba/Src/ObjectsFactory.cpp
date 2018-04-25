#include "ObjectsFactory.h"
#include "ComponentsConstructors.h"
#include "SceneManager.h"
#include "DataManager.h"    // for EntityConstructionData
#include "Entity.h"         // for Entity
#include "Component.h"
#include "Components.h"
#include "Scene.h"

 ObjectsFactory* ObjectsFactory::instance = nullptr;
 //std::string ObjectsFactory::stringIdOfPrefab[ObjectsFactory::Prefab::size];

ObjectsFactory::ObjectsFactory():
	currentId(0)
{
	instance = this;
}


ObjectsFactory::~ObjectsFactory()
{
}

Entity * ObjectsFactory::create(const EntityConstructionData & entityData, Scene* currentScene)
{
	Entity* newEntity = new Entity(currentScene, getUniqueId(), entityData.entityName);

	buildEntity(newEntity, entityData.data);

	return newEntity;
}

void ObjectsFactory::buildEntity(Entity * e, const std::vector<ComponentConstructorData> & data)
{
	for (auto c : data)
	{
		e->addComponent(buildComponent(c.type, c.componentConstructor));
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
		auto* c = static_cast<ComponentConstructors::RigidBody*>(info);
		newComponent = new RigidBody_c(c->mass);
		}	
		break;
	case ComponentType::TRANSFORM: 
	{
		auto* c = static_cast<ComponentConstructors::Transform*>(info);
		newComponent = new Transform_c(c->position[0], c->position[1], c->position[2], c->rotation[0], c->rotation[1], c->rotation[2], c->rotation[3]);
		}
		break;
	case ComponentType::COLLIDER: 
		{
		auto*p = static_cast<ComponentConstructors::Collider*>(info);
		Collider_c::Dimensions d;
		if (p->dimensions.x) d.x = p->dimensions.x;
		if (p->dimensions.y) d.y = p->dimensions.y;
		if (p->dimensions.z) d.z = p->dimensions.z;
		newComponent = new Collider_c((Collider_c::Shape)p->shape, d, btTransform(btQuaternion(btVector3(p->rotationAxis.x, p->rotationAxis.y, p->rotationAxis.z), p->rotationAngle), btVector3(p->position.x, p->position.y, p->position.z)));
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

