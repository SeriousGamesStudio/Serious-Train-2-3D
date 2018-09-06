#ifndef _H_TRAIN_HP_H_
#define  _H_TRAIN_HP_H_
#include "Component.h"
#include "Components.h"

// componente del tren que gestiona la vida
class TrainHP_c:
	public Component
{
public:
	TrainHP_c(int life);
	~TrainHP_c();

	virtual void start();
	virtual void listen(Msg_Base* msg);

	inline int getLife() { return life_; }

private:
	int life_;
};


#endif // !_H_TRAIN_HP_H_

