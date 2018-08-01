#include "Level.h"
#include "SceneManager.h"
#include "ObjectsFactory.h"
#include "Game.h"
#include "DataManager.h"
#include "GameManager_c.h"

#include <algorithm>

#include "btBulletCollisionCommon.h"
Level::Level() :
	Scene(),
	_gameManager(0)
{
	std::string sceneDataPath = "..\\Data\\Levels\\exampleAuto.xml";//Esto queda por ver cómo darle valor y tal leyendo de fichero
	SceneData* sceneData = DataManager::getInstance()->loadScene(sceneDataPath);
	for (auto entityData : *sceneData)
	{
		Entity* newEntity = ObjectsFactory::getInstance()->create(*entityData, this);
		entities.push_back(newEntity);
	}
	delete sceneData;

	GraphicsManager::getInstance()->getGUI()->createStaticImage(Ogre::Vector4(-25, -25, 50, 50), "crossAir", 
		GUIndilla::POSITION_TYPE::PT_ABSOLUTE, GUIndilla::VERTICAL_ANCHOR::VA_CENTER, GUIndilla::HORINZONTAL_ANCHOR::HA_CENTER);
}

Level::~Level()
{
	
}

