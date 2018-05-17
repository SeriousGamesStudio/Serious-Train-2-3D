#include "PlaneCollider_c.h"
#include "Components.h"


PlaneCollider_c::PlaneCollider_c(btVector3 normal):
	Component(ComponentType::PLANECOLLIDER)
{
	btCollisionShape* groundShape = new btStaticPlaneShape(normal, 0.5);
	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -3, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	PhysicsManager::getInstance()->addRigidBody(groundRigidBody);
}
PlaneCollider_c::~PlaneCollider_c(){}

void PlaneCollider_c::start() 
{

}