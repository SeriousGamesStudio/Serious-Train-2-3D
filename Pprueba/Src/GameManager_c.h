#ifndef _H_GAMEMANAGER_C_H_
#define _H_GAMEMANAGER_C_H_
#include "Component.h"

class GameManager_c: 
	public Component
{
public:
	GameManager_c();
	~GameManager_c();

	virtual void start();
	virtual void update();
	virtual void listen(Msg_Base* msg);

private:
	static void closeGame();

};



#endif // !1
