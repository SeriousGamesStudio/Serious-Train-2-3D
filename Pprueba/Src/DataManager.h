#ifndef _H_DATAMANAGER_H_
#define _H_DATAMANAGER_H_

#include <string>
#include <vector>
#include <map>

#include "ComponentsConstructors.h"

///////////////////////TYPE DECLARATIONS////////////////////////////////////////////////
enum ComponentType;
enum Prefab;
typedef std::map<ComponentType, ComponentConstructors::ComponentConstructor*> ConstructionData;

struct EntityConstructionData {
	Prefab prefab;
	ConstructionData data;
	std::string entityName;
};
typedef std::vector<EntityConstructionData> SceneData;
///////////////////////TYPE DECLARATIONS////////////////////////////////////////////////


struct GameState
{
	unsigned int score;
	//Muchas más cosis
};
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

	void saveGame(std::string path);
	void loadGame(std::string path);

	SceneData& loadScene(std::string path);

private:
	static DataManager* instance;
	DataManager();

	GameState gameState;
};

#endif //!_H_DATAMANAGER_H_

