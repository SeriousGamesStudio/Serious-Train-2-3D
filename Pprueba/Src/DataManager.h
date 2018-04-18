#ifndef _H_DATAMANAGER_H_
#define _H_DATAMANAGER_H_

#include "ObjectsFactory.h"
#include "vcruntime_new.h"   // for operator new
#include "xstring"           // for string


///////////////////////TYPE DECLARATIONS////////////////////////////////////////////////

//Container useful for ObjectsFactory
typedef std::map<ComponentType, ComponentConstructors::ComponentConstructor*> ConstructionData;

//Container useful for Scene to use into the ObjectsFactory
struct EntityConstructionData {
	ObjectsFactory::Prefab prefab;
	ConstructionData data;
	std::string entityName;
};
//Container of all the info need from the Scene to init itself
typedef std::vector<EntityConstructionData> SceneData;

///////////////////////TYPE DECLARATIONS////////////////////////////////////////////////

///////////////////////GAME STATE///////////////////////////////////////////////////////
//Current State of the Game(the gameplay way)
struct GameState
{
	unsigned int score;
	//Muchas más cosis
};
///////////////////////GAME STATE///////////////////////////////////////////////////////

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

