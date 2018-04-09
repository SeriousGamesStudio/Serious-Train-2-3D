#include "Entity.h"
#include "Components.h"

Walker_c::Walker_c():
	Component(ComponentType::CAMERA)
{
	
	rb = nullptr;
	currentDirection = btVector3();	
}

Walker_c::~Walker_c()
{
	//Has no dynamic memory allocated
}

void Walker_c::start() {
	rb = static_cast<RigidBody_c*>(_myEntity->getComponent(ComponentType::RIGIDBODY));
#ifdef _DEBUG
	if (!rb)
		printf("The entity has no 'RigidBody' component!\n");
#endif // _DEBUG


}
void Walker_c::update()
{
	// Aplicar una fuerza al rigidBody de la entidad
	// Cómo aplicamos las fuerza para que sean constantes??
	// Esto lo deberíamos haber aprendido en física, peero no lo hemos hecho
	btVector3 force = btVector3(currentDirection.absolute());
	rb->get()->applyCentralForce( force*= 50);
	// Or should we use this?
	//rb->get()->setLinearVelocity(/*vel vector*/)
}

void Walker_c::listen(Msg_Base * msg)
{
	//No necesita mensajes
}

void Walker_c::setDirection(float x, float z)
{
	currentDirection.setValue(x, 0, z);
	currentDirection.normalize();
}
