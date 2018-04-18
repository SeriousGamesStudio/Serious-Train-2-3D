#ifndef _H_DATAMANAGER_H_
#define _H_DATAMANAGER_H_

#include "ObjectsFactory.h"
#include <string>
#include <bitset>

// esto se tiene que calcular dinámicamente en tiempo de compilación,
// pero por ahora puede valer
#define STORE_WEAPONS_COUNT 10
#define LEVELS_COUNT 2

///////////////////////TYPE DECLARATIONS//////////////////////////////////////////////////////////

//Container useful for ObjectsFactory
typedef std::map<ComponentType, ComponentConstructors::ComponentConstructor*> ConstructionData;

//Container useful for Scene to use into the ObjectsFactory
struct EntityConstructionData {
	ObjectsFactory::Prefab prefab;
	ConstructionData data;
	std::string entityName;
	~EntityConstructionData() //Somos limpios e implementamos una destructora para la memoria dinámica que creamos
	{
		for (auto it = data.begin(); it != data.end(); it++) 
		{
			if (it->second) {
				delete it->second;
				it->second = nullptr;
			}
		}
	}
};
//Container of all the info need from the Scene to init itself
typedef std::vector<EntityConstructionData> SceneData;

///////////////////////TYPE DECLARATIONS//////////////////////////////////////////////////////////

///////////////////////GAME STATE/////////////////////////////////////////////////////////////////

//Current State of the Game (gameplay wise)
struct GameState
{
//////////////////////////////////////////////////////////////////////////////////////////////////
//PROHIBIDO USAR PUNTEROS. Todos los atributos tienen que ser en memoria estática contenida aquí//
//////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int score;
	unsigned int gold;
	std::bitset<STORE_WEAPONS_COUNT> storeWeapons;
	std::bitset<LEVELS_COUNT> levelsUnlocked;
	//dunno more cosis...
};
///////////////////////GAME STATE/////////////////////////////////////////////////////////////////

class DataManager
{
public:

	static DataManager* getInstance() 
	{
		if (!instance)
			instance = new DataManager();
		return instance;
	};
	~DataManager() { instance = nullptr; };


	/*******************************************************************
	 * Save the currentState of the game
	 * params:
	 *  -path: the path to the file where the game state will save
	********************************************************************/
	void saveGame(std::string path);

	/*******************************************************************
	* Load a game state from a file
	* params:
	* - path: the path to the file which have to load the game state
	*******************************************************************/
	void loadGame(std::string path);

	/*******************************************************************
	* Load the information need to build a scene
	* params:
	* - path: the path to the file wich contains the data needed 
	*         to build the scene.
	********************************************************************/
	SceneData& loadScene(std::string path);

private:
	static DataManager* instance;
	DataManager();

	GameState gameState;
};

#endif //!_H_DATAMANAGER_H_

