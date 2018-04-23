#include "Terrain_c.h"
#include "Components.h"


Terrain_c::Terrain_c():
	Component(ComponentType::TERRAIN),
	mTerrainGlobals(0),
	mTerrainGroup(0)
{
	ghM_ = GraphicsManager::getInstance();
}

Terrain_c::~Terrain_c()
{
	destroyScene();
}

void Terrain_c::createScene()
{
	ghM_->getCamera()->setPosition(Ogre::Vector3(1683, 50, 2116));
	ghM_->getCamera()->lookAt(Ogre::Vector3(1963, 50, 1660));
	ghM_->getCamera()->setNearClipDistance(0.1);
	
	bool infiniteClip =
		ghM_->getRoot()->getRenderSystem()->getCapabilities()->hasCapability(
			Ogre::RSC_INFINITE_FAR_PLANE);

	if (infiniteClip)
		ghM_->getCamera()->setFarClipDistance(0);
	else
		ghM_->getCamera()->setFarClipDistance(50000);

	ghM_->getSceneManager()->setAmbientLight(Ogre::ColourValue(.2, .2, .2));

	Ogre::Vector3 lightDir(.55, -.3, .75);
	lightDir.normalise();

	Ogre::Light* light = ghM_->getSceneManager()->createLight("TestLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightDir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(.4, .4, .4));

	// Fog
	Ogre::ColourValue fadeColour(.9, .9, .9);
	ghM_->getWindow()->getViewport(0)->setBackgroundColour(fadeColour);

	ghM_->getSceneManager()->setFog(Ogre::FOG_EXP2, fadeColour, 0.002);

	// Terrain
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
		ghM_->getSceneManager(),
		Ogre::Terrain::ALIGN_X_Z,
		513, 12000.0);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	configureTerrainDefaults(light);

	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);

	mTerrainGroup->loadAllTerrains(true);

	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();

		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup->freeTemporaryResources();

	// Sky Techniques
	// ghM_->getSceneManager()->setSkyBox(true, "Examples/SpaceSkyBox", 300, false);
	ghM_->getSceneManager()->setSkyDome(true, "Examples/CloudySky", 5, 8);
	// Ogre::Plane plane;
	// plane.d = 1000;
	// plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;

	// ghM_->getSceneManager()->setSkyPlane(
	//   true, plane, "Examples/SpaceSkyPlane", 1500, 40, true, 1.5, 150, 150);



}

void Terrain_c::createFrameListener()
{
	//GraphicsManager::createFrameListener();
}

void Terrain_c::defineTerrain(long x, long y)
{
}

void Terrain_c::initBlendMaps(Ogre::Terrain * terrain)
{
}

void Terrain_c::configureTerrainDefaults(Ogre::Light * light)
{
}
