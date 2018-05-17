#ifndef PLANE_COLLIDER_H
#define PLANE_COLLIDER_H

#include "Component.h"
#include "PhysicsManager.h"

class PlaneCollider_c:
	public Component
{
public:
	PlaneCollider_c(btVector3 normal);
	~PlaneCollider_c();

	void start();
private:

};

#endif // !PLANE_COLLIDER_H
