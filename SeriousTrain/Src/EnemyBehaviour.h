#ifndef _H_ENEMYBEHAVIOUR_H_
#define _H_ENEMYBEHAVIOUR_H_
#include "Component.h"
#include "Components.h"

// Componente que indica las caracteristicas de cada enemigo
class Walker_c;
struct Attributes
{
	int hp;
	int dmg;
	int vel;
};

// Tipos
const Attributes  normal = 
//Hp, dmg, vel
{ 150, 1 , 100};

const Attributes  fly2fly =
//Hp, dmg, vel
{ 80, 2 , 150};

class Collider_c;
class RigidBody_c;

class EnemyBehaviour_c :
	public Component
{
public:
	enum Type{ NORMAL, FLY, size };
public:
	EnemyBehaviour_c(Type t, bool frente);
	~EnemyBehaviour_c();


	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base*msg);

private:
	Attributes at;    //tipo
	Collider_c* col;  //collider
	RigidBody_c* rb;  //rigidBody
	Walker_c* wal;    //walker

	int dir;          //direccion
	

	int counter;      //temporizador feedback
	int lifeCounter_; //temporizador dmgTrain
	
};


#endif // !_H_ENEMYBEHAVIOUR_H_