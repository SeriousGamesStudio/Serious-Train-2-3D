#ifndef _H_TERRAIN_C_H_
#define _H_TERRAIN_C_H_
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "GraphicsManager.h"
#include "Component.h"


class Terrain_c:
	public Component
{
public:
	Terrain_c();
	~Terrain_c();
protected:

	virtual void createScene();
	virtual void createFrameListener();
	virtual void destroyScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

private:
	GraphicsManager* ghM_;

	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::Light* light);

	bool mTerrainsImported;
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	//OgreBites::Label* mInfoLabel;
};
#endif // !_H_TERRAIN_C_H_

