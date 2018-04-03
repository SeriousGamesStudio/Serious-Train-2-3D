#ifndef _H_GRAPHICSMANAGER_H_
#define _H_GRAPHICSMANAGER_H_


#include "Ogre.h"
#include "MeshRenderer_c.h"


class Camera_c;
class Game;

class GraphicsManager
{
public:
	GraphicsManager(Game* game);
	~GraphicsManager();
	bool start();
	void renderFrame();
	Ogre::SceneNode* createNewNode(std::string meshName);


	
	
	inline Ogre::RenderWindow* getWindow() const ;
	inline Ogre::Light* getLight() { return light; }
	inline Ogre::SceneManager* getSceneManager() { return scnMgr; }
	
	
private:
	Game* game;
	//OGRE

	Ogre::Root* root;
	Ogre::SceneManager * scnMgr;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::String mOgreCfg;
	Ogre::String mOgreLog;
	Ogre::ConfigFile cf;

	Ogre::RenderWindow* mWindow;
	
	Camera_c* cam;
	
	Ogre::Light *light;

	//PLANO
	
	
};

#endif //!_H_GRAPHICSMANAGER_H_

