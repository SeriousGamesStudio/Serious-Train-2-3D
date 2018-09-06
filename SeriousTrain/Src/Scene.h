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
#include "Components.h"
#include "MeshRenderer_c.h"

class SceneManager;
class Game;
class GameManager_c;

class Scene
{
public:

	enum Tipo {
		MENU,
		LEVEL,
		size
	};
	
	Scene(Tipo tipo);
	virtual ~Scene();

	void tick();

	void addListiner(MsgId id, Component* component);
	void removeListener(MsgId id, Component* component);


	void reciveMsg(Msg_Base* newMsg);
	//Entity management
	Entity* whoIs(EntityId id);
	Entity* whoIs(std::string name);

	void setGameManager();
	void restaEnemigo();
	
	void endGame() { endLevel_ = true; }
	


	inline GameManager_c* getGameManager() { return (GameManager_c*)_gameManager->getComponent(ComponentType::GAMEMANAGER); }


	std::string const getNameOf(EntityId id);
	EntityId const getIdOf(std::string name);

private:
	std::list <Entity*> entities;                                 // entidades en la escena	
	std::list <Entity*> graveland;								  // entidades para borrar
	std::deque<Msg_Base*> messages;								  // mensajes sin procesar
	std::vector<Msg_Base*> messagesBuffer;						  // mensajes procesados
	std::unordered_map<MsgId, std::vector<Component*>> listeners; // listeners

	Entity * _gameManager;
	Tipo t_;
	int counter_;		// temporizador de fin de nivel (para que se borren las entidades correctamente al siguiente frame)
	bool start_;		// comenzar nivel
	bool endLevel_;		// acabar nivel
	int numEnemigos_;	// num enemigos del nivel
	std::string enRes_; // texto con num enemigos
	std::string level;  // texto nivel
	

	bool isSendingMessages;
	void _msgDeliver();
	void _dumpMessages();
	void destroyEntity(Entity* entity);

};


#endif //!_H_SCENE_H_