#pragma once
#include <stdio.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include "OgreSceneNode.h"
#include <OgreConfig.h>
#include "OgreConfigFile.h"

#include <OgreTextureManager.h>
#include <OgreWindowEventUtilities.h>
#include <OgreException.h>
#include <string>
#include <iostream>
class Game
{
public:
	Game();
	virtual ~Game();
	bool init();
	bool loop();
	void close();

private:
	Ogre::Root* root;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::ConfigFile cf;

	Ogre::SceneManager * scnMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::Camera * cam;
	Ogre::Viewport * vp;
	Ogre::Light *light;

};

