/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
  -----------------------------------------------------------------------------
  Filename:    Isosurf.cpp
  Description: Demonstrates the use of the geometry shader to tessellate an 
  isosurface using marching tetrahedrons. Partial implementation of cg 
  Isosurf sample from NVIDIA's OpenGL SDK 10 : 
  http://developer.download.nvidia.com/SDK/10/opengl/samples.html
  -----------------------------------------------------------------------------
*/

#include "SdkSample.h"
#include "SamplePlugin.h"
#include "ProceduralTools.h"

using namespace Ogre;
using namespace OgreBites;

class _OgreSampleClassExport Sample_Isosurf : public SdkSample
{
    Entity* tetrahedra;
    MeshPtr mTetrahedraMesh;

 public:
        
    Sample_Isosurf() 
    { 
        mInfo["Title"] = "Isosurf";
        mInfo["Description"] = "A demo of procedural geometry manipulation using geometry shaders.";
        mInfo["Thumbnail"] = "thumb_isosurf.png";
        mInfo["Category"] = "Geometry";
    }

    StringVector getRequiredPlugins()
    {
        StringVector names;
		if(!GpuProgramManager::getSingleton().isSyntaxSupported("glsl150")
		&& !GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
            names.push_back("Cg Program Manager");
        return names;
    }

    void testCapabilities(const RenderSystemCapabilities* caps)
    {
        if (!caps->hasCapability(RSC_GEOMETRY_PROGRAM))
        {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Your render system / hardware does not support geometry programs, "
                        "so you cannot run this sample. Sorry!", 
                        "Sample_Isosurf::testCapabilities");
        }

        Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
            "Num output vertices per geometry shader run : " << caps->getGeometryProgramNumOutputVertices();
    }

    // Just override the mandatory create scene method
    void setupContent(void)
    {
        mCameraNode->setPosition(0, 0, -40);
        mCameraNode->lookAt(Vector3(0,0,0), Node::TS_PARENT);
        mCamera->setNearClipDistance(0.1);
        mCamera->setFarClipDistance(100);
        
        mTetrahedraMesh = ProceduralTools::generateTetrahedra();
        // Create tetrahedra and add it to the root scene node
        tetrahedra = mSceneMgr->createEntity("TetrahedraEntity", mTetrahedraMesh->getName());
        //tetrahedra->setDebugDisplayEnabled(true);
        Ogre::SceneNode* parentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        parentNode->attachObject(tetrahedra);
        parentNode->setScale(10,10,10);
    }

    void cleanupContent()
    {
        MeshManager::getSingleton().remove(mTetrahedraMesh->getName(), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    }

    bool frameRenderingQueued(const FrameEvent& evt)
    {
        Real seconds = (Real)(Root::getSingleton().getTimer()->getMilliseconds()) / 1000.0;
        Ogre::Pass* renderPass = tetrahedra->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0);
        if (renderPass->hasVertexProgram())
        {
            Ogre::Vector4 constParam = Ogre::Vector4(-0.5, 0.0, 0.0, 0.2);
            renderPass->getVertexProgramParameters()->setNamedConstant("Metaballs[0]", constParam);

            Ogre::Vector4 timeParam = Ogre::Vector4(
                0.1 + Ogre::Math::Sin(seconds)*0.5, Ogre::Math::Cos(seconds)*0.5, 0.0, 0.1);
            renderPass->getVertexProgramParameters()->setNamedConstant("Metaballs[1]", timeParam);
        }
        
        return SdkSample::frameRenderingQueued(evt); 
    }
};

#ifndef OGRE_STATIC_LIB

static SamplePlugin* sp;
static Sample* s;

extern "C" void _OgreSampleExport dllStartPlugin(void);
extern "C" void _OgreSampleExport dllStopPlugin(void);

extern "C" _OgreSampleExport void dllStartPlugin()
{
    s = new Sample_Isosurf;
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
