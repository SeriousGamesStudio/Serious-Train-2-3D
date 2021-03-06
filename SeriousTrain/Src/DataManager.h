#ifndef _H_DATAMANAGER_H_
#define _H_DATAMANAGER_H_

#include <string>
#include <bitset>
#include <vector>
#include "ComponentsConstructors.h"

// esto se tiene que calcular din�micamente en tiempo de compilaci�n,
// pero por ahora puede valer
#define STORE_WEAPONS_COUNT 10
#define LEVELS_COUNT 2

///////////////////////TYPE DECLARATIONS//////////////////////////////////////////////////////////

//Container useful for ObjectsFactory
//typedef std::map<ComponentType, ComponentConstructors::ComponentConstructor*> ConstructionData;
struct ComponentConstructorData 
{
	ComponentConstructorData() {};
	ComponentConstructorData(ComponentType _type, ComponentConstructors::ComponentConstructor* cc):
	type(_type), componentConstructor(cc){};
	ComponentType type;//Enum
	ComponentConstructors::ComponentConstructor* componentConstructor;//abstract type to polymorfism
};
//Container useful for Scene to use into the ObjectsFactory
struct EntityConstructionData {
	std::vector<ComponentConstructorData> data;
	std::string entityName;
	~EntityConstructionData() //We're clean and free the dynamic memory of ea
	{
		for (ComponentConstructorData e : data)
			if (e.componentConstructor)
			delete e.componentConstructor;
	}
};
//Container of all the info need from the Scene to init itself
//typedef std::vector<EntityConstructionData*> SceneData;
struct SceneData :
	public std::vector<EntityConstructionData*>
{
	~SceneData() 
	{
		for (auto* e : *this)
		{
			delete e;
		}
	}
};

///////////////////////TYPE DECLARATIONS//////////////////////////////////////////////////////////

///////////////////////GAME STATE/////////////////////////////////////////////////////////////////

//Current State of the Game (gameplay wise)
struct GameState
{
//////////////////////////////////////////////////////////////////////////////////////////////////
//PROHIBIDO USAR PUNTEROS. Todos los atributos tienen que ser en memoria est�tica contenida aqu�//
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
	SceneData* loadScene(std::string path);

private:
	static DataManager* instance;
	DataManager();

	GameState gameState;
};

#endif //!_H_DATAMANAGER_H_

