#ifndef _H_GRAPHICSMANAGER_H_
#define _H_GRAPHICSMANAGER_H_


#include "Ogre.h"
#include "MeshRenderer_c.h"
#include "OgreOverlaySystem.h"
#include <OgreOverlayManager.h>
#include "GUIndilla.h"

class Game;

class GraphicsManager
{
public:
	~GraphicsManager();
	bool start();
	void renderFrame();
	Ogre::SceneNode* createNewNode(std::string meshPath, std::string meshName);

	void initGUI();
	GUIndilla::GUI *getGUI() {
		if (!_GUI) {
			initGUI();
		}
		return _GUI;
	};

	void toggleMouse(bool toggle);
	
	 Ogre::RenderWindow* getWindow() const ;
	 Ogre::Light* getLight() { return light; }
	 Ogre::SceneManager* getSceneManager() { return scnMgr; }
	 Ogre::Camera* getCamera() { return cam; }
	
	inline Ogre::Entity* getOgreEntity() { return ent_; }
	static GraphicsManager* getInstance();
private:
	GraphicsManager();
	static GraphicsManager* instance;

	//OGRE
	Ogre::Root* root;
	Ogre::SceneManager * scnMgr;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::String mOgreCfg;
	Ogre::String mOgreLog;
	Ogre::ConfigFile cf;
	Ogre::Entity* ent_;

	Ogre::RenderWindow* mWindow;
	Ogre::Camera * cam;
	Ogre::Viewport * vp;
	
	Ogre::Light *light;

	Ogre::OverlaySystem * overlaySystem;
	Ogre::OverlayManager* overlayManager;

	GUIndilla::GUI *_GUI;
	//PLANO
	
	
};

#endif //!_H_GRAPHICSMANAGER_H_

