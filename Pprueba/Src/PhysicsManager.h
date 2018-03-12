#ifndef _H_PHYSICSMANAGER_H_
#define _H_PHYSICSMANAGER_H_


class Game;

class PhysicsManager
{
public:
	PhysicsManager(Game* game);
	~PhysicsManager();

	void stepUp();

private:

};
#endif //!_H_PHYSICSMANAGER_H_

