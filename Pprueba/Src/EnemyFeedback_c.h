#ifndef _H_ENEMYFEEDBACK_H_
#define _H_ENEMYFEEDBACK_H_
#include "Component.h"
#include "Components.h"
#include "MeshRenderer_c.h"


class EnemyFeedback_c:
	public Component
{
public:
	EnemyFeedback_c();
	~EnemyFeedback_c();

	void start();

	virtual void listen(Msg_Base* msg);
	

private:
	MeshRenderer_c * mesh_;
};
#endif // !_H_ENEMYFEEDBACK_H_
