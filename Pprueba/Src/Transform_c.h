#ifndef _H_TRANSFORM_C_H_
#define _H_TRANSFORM_C_H_

#include "PhysicsManager.h"
#include "Component.h"

class Transform_c :
	public Component
{
public:

	Transform_c(const btVector3& position, const btQuaternion& rotation);
	Transform_c(float px, float py, float pz, float qx, float qy, float qz, float qw);
	virtual ~Transform_c();

	inline btTransform& get() { return t; };
	inline const btTransform& get() const { return t; };

private:
	btTransform t;

};
#endif //!_H_TRANSFORM_C_H_
