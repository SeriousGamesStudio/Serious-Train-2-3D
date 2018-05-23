#include "GraphicsManager.h"
#include "Game.h"
#include "TrashCollector.h"

GraphicsManager* GraphicsManager::instance = nullptr;

GraphicsManager::GraphicsManager() :
	root(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK)
{

}

GraphicsManager::~GraphicsManager()
{
	delete _GUI;
	delete overlaySystem;
	delete root;
	instance = nullptr;
}

bool GraphicsManager::start()
{

#ifdef _DEBUG
	mResourcesCfg = "./OgreD/resources_d.cfg";
	mPluginsCfg = "./OgreD/plugins_d.cfg";
#else
	mResourcesCfg = "Ogre/resources.cfg";
	mPluginsCfg = "Ogre/plugins.cfg";
#endif

	root = new Ogre::Root(mPluginsCfg);

	overlaySystem = new Ogre::OverlaySystem();
	overlayManager = Ogre::OverlayManager::getSingletonPtr();
	//Rutas de recursos y plugins



	cf.load(mPluginsCfg);
	//Setting UP Resources 

	//Parsing the config file into the system.
	cf.load(mResourcesCfg);


	//name: Path to resources in disk,
	//loctype: defines what kind of location the element is (e.g. Filesystem, zip..)
	Ogre::String name, locType;

	//We now iterate through rach section in the resources.cfg.
	//Sections are signaled as [NAME]
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		//Now we are iterating INSIDE the section [secIt]
		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;

			//We now know the type of the element and its path.
			//We add it as a location to the Resource Group Manager
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
			std::cout << "inserting:\n";
			std::cout << name << locType << std::endl;
		}
	}
	//------------------------------------------------------------------------------------------------------
	// Render System Config

	//If there is no previous Ogre.cfg, this displays the config dialog
	if (!(root->restoreConfig() || root->showConfigDialog()))
		return false;

	//------------------------------------------------------------------------------------------------------
	//Render Window Creation
	mWindow = root->initialise(true, "Serious Train xd");

	//------------------------------------------------------------------------------------------------------
	//Resources Init

	//We are only going to use 5 mimpams at a time. Mipmaps are efficent ways to save a texture.
	//Taking only 1/3 more of space, we can have several sizes of the texture to choose from.
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//Now we init every resource previously added
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//------------------------------------------------------------------------------------------------------
	//SceneManager Set Up

	//we generate the default sceneManager. (more SceneManagers in Ogre::ST_....)
	scnMgr = root->createSceneManager(Ogre::ST_GENERIC);
	scnMgr->addRenderQueueListener(overlaySystem);
	//panel->setMaterialName("MaterialName"); // Optional background material

	//------------------------------------------------------------------------------------------------------
	//Camera Creation
	cam = scnMgr->createCamera("MainCam");
	cam->setPosition(0, 0, 80);
	cam->lookAt(0, 0, -300);
	cam->setNearClipDistance(5);
	//Self-explanatory methods
	vp = mWindow->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	
	cam->setAspectRatio(
	Ogre::Real(vp->getActualWidth()) /
	Ogre::Real(vp->getActualHeight()));

	//------------------------------------------------------------------------------------------------------
	_GUI = nullptr;


	//------------------------------------------------------------------------------------------------------
	//Scene SetUp

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
	

	return true;
}


void GraphicsManager::toggleMouse(bool toggle)
{
	getGUI()->getMousePointer()->setActive(toggle);
}

Ogre::RenderWindow*  GraphicsManager::getWindow() const {
	return mWindow;
}

GraphicsManager * GraphicsManager::getInstance()
{
	if (!instance) {
		instance = new GraphicsManager();
		instance->start();
	}
	return instance;
 }

void GraphicsManager::renderFrame()
{
	Ogre::WindowEventUtilities::messagePump();
	if (mWindow->isClosed()) {
		Game::getInstance()->stop();
		return;
	}
	if (!root->renderOneFrame())
		return;
}

Ogre::SceneNode * GraphicsManager::createNewNode(std::string meshName)
{
	Ogre::SceneNode* newNode = nullptr;
	try {
		newNode = scnMgr->getRootSceneNode()->createChildSceneNode();
		newNode->attachObject(scnMgr->createEntity(meshName));
	}
	catch (Ogre::FileNotFoundException e) {
		std::string a = e.getFullDescription();
		std::cout << a;
		newNode = nullptr;
	}

	return newNode;
}

void GraphicsManager::initGUI()
{
	if (!_GUI) {
		_GUI = new GUIndilla::GUI("Value", 16);
		_GUI->createMousePointer(Ogre::Vector2(32, 32), "bgui.pointer");
	}
}
