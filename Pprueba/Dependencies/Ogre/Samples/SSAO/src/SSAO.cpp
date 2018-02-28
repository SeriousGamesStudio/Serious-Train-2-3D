#include "SamplePlugin.h"
#include "SSAO.h"

static SamplePlugin* sp;
static Sample* s;

extern "C" void _OgreSampleExport dllStartPlugin(void);
extern "C" void _OgreSampleExport dllStopPlugin(void);

extern "C" _OgreSampleExport void dllStartPlugin()
{
    s = new Sample_SSAO;
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
