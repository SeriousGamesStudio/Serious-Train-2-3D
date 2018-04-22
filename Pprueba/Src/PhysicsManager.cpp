#include "PhysicsManager.h"
#include "Game.h"


PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager::PhysicsManager()
{}

void PhysicsManager::init() {
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
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadPhase;
	delete world;
	instance = nullptr;
}

PhysicsManager * PhysicsManager::getInstance()
{
	if (!instance) {
		instance = new PhysicsManager();
		instance->init();
	}
	return instance;
}

void PhysicsManager::stepUp(double deltaTime/*seconds*/)
{
	//deltaTime = 0.0016;
	unsigned char maxSubSteps = 10;	//esto hay que calcularlo de manera dinámica, pero no sé como
	world->stepSimulation((float)deltaTime, maxSubSteps);
}

void PhysicsManager::addCollisionShape(btCollisionObject * newCollisionObject)
{
	if (world)
		world->addCollisionObject(newCollisionObject);
}

void PhysicsManager::removeCollisionShape(btCollisionObject * co)
{
	if (world)
		world->removeCollisionObject(co);
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
