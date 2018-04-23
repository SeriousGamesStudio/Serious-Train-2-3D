/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef __Ogre_TerrainMaterialGenerator_H__
#define __Ogre_TerrainMaterialGenerator_H__

#include "OgreTerrainPrerequisites.h"
#include "OgrePixelFormat.h"
#include "OgreMaterial.h"
#include "OgreTexture.h"

namespace Ogre
{
    class Terrain;

    enum TerrainLayerSamplerSemantic
    {
        TLSS_ALBEDO = 0,
        TLSS_NORMAL = 1,
        TLSS_HEIGHT = 2,
        TLSS_SPECULAR = 3
    };

    struct _OgreTerrainExport TerrainLayerSamplerElement
    {
        uint8 source;
        TerrainLayerSamplerSemantic semantic;
        uint8 elementStart;
        uint8 elementCount;

        bool operator==(const TerrainLayerSamplerElement& e) const
        {
            return source == e.source &&
                semantic == e.semantic &&
                elementStart == e.elementStart &&
                elementCount == e.elementCount;
        }

        TerrainLayerSamplerElement() :
            source(0), semantic(TLSS_ALBEDO), elementStart(0), elementCount(0)
        {}

        TerrainLayerSamplerElement(uint8 src, TerrainLayerSamplerSemantic sem,
            uint8 elemStart, uint8 elemCount)
            : source(src), semantic(sem), elementStart(elemStart), elementCount(elemCount)
        {
        }
    };
    typedef vector<TerrainLayerSamplerElement>::type TerrainLayerSamplerElementList;

    struct _OgreTerrainExport TerrainLayerSampler
    {
        String alias;
        PixelFormat format;

        bool operator==(const TerrainLayerSampler& s) const
        {
            return alias == s.alias && format == s.format;
        }

        TerrainLayerSampler()
            : alias(""), format(PF_UNKNOWN)
        {
        }

        TerrainLayerSampler(const String& aliasName, PixelFormat fmt)
: alias(aliasName), format(fmt)
        {
        }
    };
    typedef vector<TerrainLayerSampler>::type TerrainLayerSamplerList;

    struct _OgreTerrainExport TerrainLayerDeclaration
    {
        TerrainLayerSamplerList samplers;
        TerrainLayerSamplerElementList elements;

        bool operator==(const TerrainLayerDeclaration& dcl) const
        {
            return samplers == dcl.samplers && elements == dcl.elements;
        }
    };

    class _OgreTerrainExport TerrainMaterialGenerator : public TerrainAlloc
    {
    public:
        class _OgreTerrainExport Profile : public TerrainAlloc
        {
        protected:
            TerrainMaterialGenerator* mParent;
            String mName;
            String mDesc;
        public:
            Profile(TerrainMaterialGenerator* parent, const String& name, const String& desc)
: mParent(parent), mName(name), mDesc(desc) {}
            Profile(const Profile& prof)
: mParent(prof.mParent), mName(prof.mName), mDesc(prof.mDesc) {}
            virtual ~Profile() {}
            TerrainMaterialGenerator* getParent() const { return mParent; }
            const String& getName() const { return mName; }
            const String& getDescription() const { return mDesc; }
            virtual bool isVertexCompressionSupported() const = 0;
            virtual MaterialPtr generate(const Terrain* terrain) = 0;
            virtual MaterialPtr generateForCompositeMap(const Terrain* terrain) = 0;
            virtual void setLightmapEnabled(bool enabled) = 0;
            virtual uint8 getMaxLayers(const Terrain* terrain) const = 0;
            virtual void updateCompositeMap(const Terrain* terrain, const Rect& rect);

            virtual void updateParams(const MaterialPtr& mat, const Terrain* terrain) = 0;
            virtual void updateParamsForCompositeMap(const MaterialPtr& mat, const Terrain* terrain) = 0;

            virtual void requestOptions(Terrain* terrain) = 0;

        };

        TerrainMaterialGenerator();
        virtual ~TerrainMaterialGenerator();

        typedef vector<Profile*>::type ProfileList;

        virtual const ProfileList& getProfiles() const { return mProfiles; }

        virtual void setActiveProfile(const String& name)
        {
            if (!mActiveProfile || mActiveProfile->getName() != name)
            {
                for (ProfileList::iterator i = mProfiles.begin(); i != mProfiles.end(); ++i)
                {
                    if ((*i)->getName() == name)
                    {
                        setActiveProfile(*i);
                        break;
                    }
                }
            }

        }

        virtual void setActiveProfile(Profile* p)
        {
            if (mActiveProfile != p)
            {
                mActiveProfile = p;
                _markChanged();
            }
        }
        Profile* getActiveProfile() const
        {
            // default if not chosen yet
            if (!mActiveProfile && !mProfiles.empty())
                mActiveProfile = mProfiles[0];

            return mActiveProfile;
        }

        void _markChanged() { ++mChangeCounter; }

        unsigned long long int getChangeCount() const { return mChangeCounter; }

        virtual const TerrainLayerDeclaration& getLayerDeclaration() const { return mLayerDecl; }
        virtual bool canGenerateUsingDeclaration(const TerrainLayerDeclaration& decl)
        {
            return decl == mLayerDecl;
        }

        virtual bool isVertexCompressionSupported() const
        {
            return getActiveProfile()->isVertexCompressionSupported();
        }

        virtual void requestOptions(Terrain* terrain)
        {
            Profile* p = getActiveProfile();
            if (p)
                p->requestOptions(terrain);

        }
        virtual MaterialPtr generate(const Terrain* terrain)
        {
            Profile* p = getActiveProfile();
            if (!p)
                return MaterialPtr();
            else
                return p->generate(terrain);
        }
        virtual MaterialPtr generateForCompositeMap(const Terrain* terrain)
        {
            Profile* p = getActiveProfile();
            if (!p)
                return MaterialPtr();
            else
                return p->generateForCompositeMap(terrain);
        }
        virtual void setLightmapEnabled(bool enabled)
        {
            Profile* p = getActiveProfile();
            if (p)
                return p->setLightmapEnabled(enabled);
        }
        virtual uint8 getMaxLayers(const Terrain* terrain) const
        {
            Profile* p = getActiveProfile();
            if (p)
                return p->getMaxLayers(terrain);
            else
                return 0;
        }

        virtual void updateCompositeMap(const Terrain* terrain, const Rect& rect)
        {
            Profile* p = getActiveProfile();
            if (!p)
                return;
            else
                p->updateCompositeMap(terrain, rect);
        }


        virtual void updateParams(const MaterialPtr& mat, const Terrain* terrain)
        {
            Profile* p = getActiveProfile();
            if (p)
                p->updateParams(mat, terrain);
        }
        virtual void updateParamsForCompositeMap(const MaterialPtr& mat, const Terrain* terrain)
        {
            Profile* p = getActiveProfile();
            if (p)
                p->updateParamsForCompositeMap(mat, terrain);
        }

        virtual void setDebugLevel(unsigned int dbg)
        {
            if (mDebugLevel != dbg)
            {
                mDebugLevel = dbg;
                _markChanged();
            }
        }
        virtual unsigned int getDebugLevel() const { return mDebugLevel; }

        virtual void _renderCompositeMap(size_t size, const Rect& rect,
            const MaterialPtr& mat, const TexturePtr& destCompositeMap);

        Texture* _getCompositeMapRTT() { return mCompositeMapRTT; }
    protected:

        ProfileList mProfiles;
        mutable Profile* mActiveProfile;
        unsigned long long int mChangeCounter;
        TerrainLayerDeclaration mLayerDecl;
        unsigned int mDebugLevel;
        SceneManager* mCompositeMapSM;
        Camera* mCompositeMapCam;
        Texture* mCompositeMapRTT; // deliberately holding this by raw pointer to avoid shutdown issues
        ManualObject* mCompositeMapPlane;
        Light* mCompositeMapLight;



    };

    typedef SharedPtr<TerrainMaterialGenerator> TerrainMaterialGeneratorPtr;

}
#endif