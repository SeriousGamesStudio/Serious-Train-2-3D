#ifndef _H_GRAPHICSMANAGER_H_
#define _H_GRAPHICSMANAGER_H_


#include "Ogre.h"
#include "MeshRenderer_c.h"


using namespace Ogre;
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

	Root* root;
	SceneManager * scnMgr;
	String mResourcesCfg;
	String mPluginsCfg;
	String mOgreCfg;
	String mOgreLog;
	ConfigFile cf;

	RenderWindow* mWindow;
	Camera * cam;
	Viewport * vp;
	Light *light;

	//PLANO
	m_Entity* plano;
	MeshRenderer_c::ConstructorInfo infoPlane;
};

#endif //!_H_GRAPHICSMANAGER_H_

