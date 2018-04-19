#ifndef _H_SCENE_H_
#define _H_SCENE_H_
#pragma once

#include <list>
#include <deque>
#include <vector>
#include <unordered_map>


#include "Component.h"
#include "Notifications.h"
#include "Entity.h"

#include "MeshRenderer_c.h"

class SceneManager;
class Game;

class Scene
{
public:

	Scene();
	virtual ~Scene();

	void tick();

	void addListiner(MsgId id, Component* component);
	void removeListener(MsgId id, Component* component);


	void reciveMsg(Msg_Base* newMsg);
	//Entity management
	Entity* whoIs(EntityId id);
	Entity* whoIs(std::string name);

	void setGameManager();

	inline GameManager_c* getGameManager() { return (GameManager_c*)_gameManager->getComponent(ComponentType::GAMEMANAGER); }


	std::string const getNameOf(EntityId id);
	EntityId const getIdOf(std::string name);

private:
	std::list <Entity*> entities;
	std::deque<Msg_Base*> messages;
	std::vector<Msg_Base*> messagesBuffer;
	std::unordered_map<MsgId, std::vector<Component*>> listeners;

	Entity * _gameManager;

	bool isSendingMessages;
	void _msgDeliver();
	void _dumpMessages();
};


#endif //!_H_SCENE_H_