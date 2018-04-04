#ifndef _H_TRANSFORM_C_H_
#define _H_TRANSFORM_C_H_

#include "PhysicsManager.h"
#include "Component.h"

class Trasform_c :
	public Component
{
public:
	struct ConstructorInfo
	{
		btVector3 v;
		btQuaternion q;
	};

	Trasform_c(Entity* entity, ConstructorInfo info);
	virtual ~Trasform_c();

	inline btTransform& get() { return t; };

	virtual void start();
private:
	btTransform t;

};
#endif //!_H_TRANSFORM_C_H_
