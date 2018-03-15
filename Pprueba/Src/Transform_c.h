#ifndef _H_TRANSFORM_C_H_
#define _H_TRANSFORM_C_H_

#include "PhysicsManager.h"
#include "Component.h"

class Trasform_c :
	Component, btTransform
{
public:
	struct ConstructorInfo
	{
		btVector3 v;
		btQuaternion q;
	};

	Trasform_c(Entity* entity, PhysicsManager* physicsManager, ConstructorInfo info);
	virtual ~Trasform_c();


	virtual void start();
private:
	PhysicsManager * _pM;

};
#endif //!_H_TRANSFORM_C_H_

