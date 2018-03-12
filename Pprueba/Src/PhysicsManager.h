#ifndef _H_PHYSICSMANAGER_H_
#define _H_PHYSICSMANAGER_H_

class Game;
#include <btBulletCollisionCommon.h>

class PhysicsManager
{
public:
	PhysicsManager(Game* game);
	~PhysicsManager();

	void stepUp();

private:

};
#endif //!_H_PHYSICSMANAGER_H_

