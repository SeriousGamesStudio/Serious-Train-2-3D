#ifndef _H_COLLIDER_C_H_
#define _H_COLLIDER_C_H_

#include "PhysicsManager.h"
#include "Component.h"

class Transform_c;

class Collider_c :
	public Component
{
	enum class Shape 
	{
		SPHERE, 
		BOX,
		CYLINDER,
		CAPSULE
	};

	union Dimensions
	{
		float x;
		float y;
		float z;
	};


public:
	Collider_c(Shape shape, Dimensions dim, btTransform& trans);
	virtual ~Collider_c();

	virtual void start();

	virtual void  update();

	inline btCollisionShape* getCollisionShape() const { return _collisionShape; };
	inline btCollisionObject& getCollisionObject() { return _collisionObject; };
	inline const btCollisionObject& getCollisionObject() const { return _collisionObject; };

	void setPosition(const float x, const float y, const float z);
	void setPosition(const btVector3& v);
	void setPosition(const btTransform& trans);

	void setRotation(const float w, const float x, const float y, const float z);
	void setRotation(const btVector3& axis, const float angle);
	void setRotation(const btQuaternion& q);
	void setRotation(const btTransform& trans);

	void setTransform(const btTransform& trans);
	void setTransform(const Transform_c& trans);

private:
	Transform_c* entityTransform;
	btCollisionShape* _collisionShape;
	btCollisionObject _collisionObject;

};
#endif //!_H_COLLIDER_C_H_

