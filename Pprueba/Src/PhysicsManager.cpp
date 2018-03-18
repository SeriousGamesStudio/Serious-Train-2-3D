#include "PhysicsManager.h"
#include "Game.h"


PhysicsManager::PhysicsManager(Game* game) :
	_game(game)
{
	//build the broadPhase
	broadPhase = new btDbvtBroadphase();

	//Set up the collision configuration and dispacher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//the actual physics solver
	solver = new btSequentialImpulseConstraintSolver();

	//the world
	world = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);
	world->setGravity(btVector3(0, -10, 0));
}

PhysicsManager::~PhysicsManager()
{

	for (auto rb : rigidBodies) delete rb;
	delete world;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadPhase;
}

void PhysicsManager::stepUp(double deltaTime/*seconds*/)
{
	unsigned char maxSubSteps = 10;	//esto hay que calcularlo de manera dinámica, pero no sé como
	world->stepSimulation(deltaTime, maxSubSteps);
}

void PhysicsManager::addRigidBody(btRigidBody * newRigidBody)
{
	if (world)
		world->addRigidBody(newRigidBody);
}

void PhysicsManager::removeRigidBody(btRigidBody * rb)
{
	if (world)
		world->removeRigidBody(rb);
}
