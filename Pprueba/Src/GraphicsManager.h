#ifndef _H_GRAPHICSMANAGER_H_
#define _H_GRAPHICSMANAGER_H_


#include "Ogre.h"
#include "MeshRenderer_c.h"


class Game;

class GraphicsManager
{
public:
	GraphicsManager(Game* game);
	~GraphicsManager();
	bool start();
	void renderFrame();
	Ogre::SceneNode* createNewNode(std::string meshName);

	Ogre::RenderWindow* getWindow() const ;
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
	Ogre::Camera * cam;
	Ogre::Viewport * vp;
	Ogre::Light *light;

	//PLANO
	
	
};

#endif //!_H_GRAPHICSMANAGER_H_

