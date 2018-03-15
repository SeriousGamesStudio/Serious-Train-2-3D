#ifndef _H_COLLIDER_C_H_
#define _H_COLLIDER_C_H_

#include "PhysicsManager.h"
#include "Component.h"

class Collider_c :
	Component
{
public:
	Collider_c(Entity* entity, PhysicsManager* physicsManager, btCollisionShape* shape);
	virtual ~Collider_c();

	inline btCollisionShape* get() { return _shape; };

private:
	PhysicsManager * _pM;
	btCollisionShape * _shape;

};
#endif //!_H_COLLIDER_C_H_

