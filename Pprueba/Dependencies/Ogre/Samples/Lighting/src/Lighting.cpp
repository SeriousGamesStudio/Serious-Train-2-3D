#include "SamplePlugin.h"
#include "Lighting.h"

using namespace Ogre;
using namespace OgreBites;

#ifndef OGRE_STATIC_LIB

static SamplePlugin* sp;
static Sample* s;

extern "C" _OgreSampleExport void dllStartPlugin();
extern "C" _OgreSampleExport void dllStopPlugin();

extern "C" _OgreSampleExport void dllStartPlugin()
{
    s = new Sample_Lighting;
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
