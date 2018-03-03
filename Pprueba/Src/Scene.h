#ifndef _H_SCENE_H_
#define _H_SCENE_H_

#include <list>
#include <deque>
#include <vector>
#include <unordered_map>

#include "GameManager.h"
#include "Notifications.h"

class Scene
{
public:
	Scene(GameManager* gameManager_);
	virtual ~Scene();

	void tick();

	void addListiner(MsgId id, Component* component);
	void removeListener(MsgId id, Component* component);


	void reciveMsg(Msg& newMsg);
	//Entity management
	Entity* whoIs(EntityId id);
	Entity* whoIs(std::string name);

	std::string const getNameOf(EntityId id);
	EntityId const getIdOf(std::string name);

	template<typename T>
	Component* getComponentOf<T>(EntityId id);
	template<typename T>
	Component* getComponentOf<T>(std::string name);

private:
	GameManager * gameManager;
	std::list <Entity*> entities;

	std::deque<Msg&> messages;
	std::vector<Msg&> messagesBuffer;
	std::unordered_map<MsgId, std::vector<Component*>> listeners;
	bool isSendingMessages;
private:
	void _msgDeliver();
	void _dumpMessages();
};


#endif //!_H_SCENE_H_