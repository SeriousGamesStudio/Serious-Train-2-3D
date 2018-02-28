#include "SamplePlugin.h"
#include "Tessellation.h"

using namespace Ogre;
using namespace OgreBites;

#ifndef OGRE_STATIC_LIB

static SamplePlugin* sp;
static Sample* s;

extern "C" void _OgreSampleExport dllStartPlugin(void);
extern "C" void _OgreSampleExport dllStopPlugin(void);

extern "C" _OgreSampleExport void dllStartPlugin()
{
    s = new Sample_Tessellation;
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
