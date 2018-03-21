#ifndef _H_PHYSICS_MANAGER_H_
#define _H_PHYSICS_MANAGER_H_

#include <vector>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class Game;
using uint = unsigned int;

class PhysicsManager
{
public:
	~PhysicsManager();
	PhysicsManager(Game* game);

	void stepUp(double deltaTime/*provisional, para evitar errores de compilación*/);

	void addRigidBody(btRigidBody* newRigidBody);
	void removeRigidBody(btRigidBody* rb);

private:
	Game * _game;
	btDiscreteDynamicsWorld* world;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btBroadphaseInterface* broadPhase;

	std::vector<btRigidBody*> rigidBodies;
};
#endif //!_H_PHYSICS_MANAGER_H_

