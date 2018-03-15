#ifndef _H_GRAPHICSMANAGER_H_
#define _H_GRAPHICSMANAGER_H_


#include "Ogre.h"
class Game;

class GraphicsManager
{
public:
	GraphicsManager(Game* game);
	~GraphicsManager();
	bool start();
	void run();

private:

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
};

#endif //!_H_GRAPHICSMANAGER_H_

