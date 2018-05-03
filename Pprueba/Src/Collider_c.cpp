#include "Collider_c.h"
#include "Component.h"
#include "Components.h"
#include "Entity.h"
#include "Transform_c.h"
#include "PhysicsManager.h"


Collider_c::Collider_c(Shape shape, Dimensions dim, btTransform trans) :
	Component(ComponentType::COLLIDER), _collisionShape(nullptr), _collisionObject(btCollisionObject())
{
	switch (shape)
	{
	case Collider_c::Shape::SPHERE:
		_collisionShape = new btSphereShape((float)dim.x);
		break;
	case Collider_c::Shape::BOX:
		_collisionShape = new btBoxShape(btVector3(dim.x/2, dim.y/2, dim.z/2));
		break;
	case Collider_c::Shape::CYLINDER:
		_collisionShape = new btCylinderShape(btVector3(dim.x/2, dim.y/2, dim.z/2));
		break;
	case Collider_c::Shape::CAPSULE:
		_collisionShape = new btCapsuleShape(dim.x, dim.y);
		break;
	default:
		break;
	}
	if (!_collisionShape)
		printf("ERROR: No se ha podido inicializar el collisionShape");
	else 
	{
		_collisionObject.setCollisionShape(_collisionShape);
		_collisionObject.setWorldTransform(trans);
	}

	PhysicsManager::getInstance()->addCollisionShape(&_collisionObject);

}

Collider_c::~Collider_c()
{
	delete _collisionShape;
	PhysicsManager::getInstance()->removeCollisionShape(&_collisionObject);
}

void Collider_c::start()
{
	entityTransform = static_cast<Transform_c*>(_myEntity->getComponent(ComponentType::TRANSFORM));
#ifdef _DEBUG	
	if (!entityTransform)
		printf("ERROR: La entidad no contiene el componente Transform_c");
#endif
}

void Collider_c::update()
{
	setPosition(entityTransform->get());
}

void Collider_c::setPosition(const float x, const float y, const float z)
{
	setPosition(btVector3(x, y, z));
}

void Collider_c::setPosition(const btVector3 & v)
{
	setPosition(btTransform(_collisionObject.getWorldTransform().getRotation(), v));
}

void Collider_c::setPosition(const btTransform & trans)
{
	_collisionObject.setWorldTransform(trans);
}

void Collider_c::setRotation(const float w, const float x, const float y, const float z)
{
	setRotation(btQuaternion(x, y, z, w));
}

void Collider_c::setRotation(const btVector3 & axis, const float angle)
{
	setRotation(btQuaternion(axis, angle));
}

void Collider_c::setRotation(const btQuaternion & q)
{
	setRotation(btTransform(q, _collisionObject.getWorldTransform().getOrigin()));
}

void Collider_c::setRotation(const btTransform & trans)
{
	_collisionObject.setWorldTransform(trans);
}

void Collider_c::setTransform(const btTransform & trans)
{
	_collisionObject.setWorldTransform(trans);
}

void Collider_c::setTransform(const Transform_c & trans)
{
	_collisionObject.setWorldTransform(trans.get());
}
