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

	Scene(SceneManager* sceneManager_, Game* game);
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

	/*Por ahora no
	template<typename T>
	Component* getComponentOf<T>(EntityId id);
	template<typename T>
	Component* getComponentOf<T>(std::string name);
	*/

private:
	SceneManager * sceneManager;
	std::list <Entity*> entities;
	Game* game_;

	std::deque<Msg> messages;
	std::vector<Msg> messagesBuffer;
	std::unordered_map<MsgId, std::vector<Component*>> listeners;
	bool isSendingMessages;
	void _msgDeliver();
	void _dumpMessages();	
};


#endif //!_H_SCENE_H_