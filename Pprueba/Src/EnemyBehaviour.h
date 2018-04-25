#ifndef _H_ENEMYBEHAVIOUR_H_
#define _H_ENEMYBEHAVIOUR_H_
#include "Component.h"

struct Attributes
{
	int hp;
	int dmg;
	int vel;
};

const Attributes  normal = 
//Hp, dmg, vel
{ 100, 50 , 100};

const Attributes  fly2fly =
//Hp, dmg, vel
{ 80, 50 , 150};

class Collider_c;

class EnemyBehaviour_c :
	public Component
{
public:
	enum Type{ NORMAL, FLY};
public:
	EnemyBehaviour_c(Type t);
	~EnemyBehaviour_c();


	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base*msg);

private:
	Attributes at;
	Collider_c* col;

};


#endif // !_H_ENEMYBEHAVIOUR_H_