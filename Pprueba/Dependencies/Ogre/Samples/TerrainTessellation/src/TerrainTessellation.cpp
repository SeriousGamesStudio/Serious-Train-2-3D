#include "SamplePlugin.h"
#include "TerrainTessellation.h"

using namespace Ogre;
using namespace OgreBites;

#ifndef OGRE_STATIC_LIB

static SamplePlugin* sp;
static Sample* s;

extern "C" _OgreSampleExport void dllStartPlugin()
{
    s = new Sample_TerrainTessellation;
    sp = OGRE_NEW SamplePlugin(s->getInfo()["Title"] + " Sample");
    sp->addSample(s);
    Root::getSingleton().installPlugin(sp);
}

extern "C" _OgreSampleExport void dllStopPlugin()
{
    Root::getSingleton().uninstallPlugin(sp); 
    OGRE_DELETE sp;
    delete s;
}
#endif
