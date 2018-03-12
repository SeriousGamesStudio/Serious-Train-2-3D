#ifndef _H_INPUTMANAGER_H_
#define _H_INPUTMANAGER_H_

#include <map>
#include <deque>
#include <functional>
class Game;

enum class INPUT_NOTIFICATION : unsigned int
{
	MOUSE_OVER,
	LEFT_CLICK,
	RIGHT_CLICK,
	A_PRESSED,
	D_PRESSED,
};

class InputManager
{
public:
	InputManager(Game* game);
	~InputManager();

	void update();


	void subscribe(INPUT_NOTIFICATION notification, std::function<void> callback);
	void unsubscribe(INPUT_NOTIFICATION notification, std::function<void> callback);


private:
	void parseEvents();
	void dispachNotificacations();

private:
	std::deque <INPUT_NOTIFICATION> notifications;
	std::map < INPUT_NOTIFICATION, std::vector<std::function<void>>> subscribers;
};
#endif //!_H_INPUTMANAGER_H_

