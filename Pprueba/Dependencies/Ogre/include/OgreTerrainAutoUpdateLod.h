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

#ifndef __Ogre_TerrainAutoUpdateLod_H__
#define __Ogre_TerrainAutoUpdateLod_H__

#include "OgreTerrainPrerequisites.h"
#include "OgreTerrain.h"

namespace Ogre
{
    class _OgreTerrainExport TerrainAutoUpdateLod : public TerrainAlloc
    {
    public:
        virtual ~TerrainAutoUpdateLod() {}
        virtual void autoUpdateLod(Terrain *terrain, bool synchronous, const Any &data) = 0;
        virtual uint32 getStrategyId() = 0;
    };

    // other Strategy's id start from 2
    enum TerrainAutoUpdateLodStrategy
    {
        NONE = 0,
        BY_DISTANCE = 1
    };

    class _OgreTerrainExport TerrainAutoUpdateLodByDistance : public TerrainAutoUpdateLod
    {
    public:
        virtual void autoUpdateLod(Terrain *terrain, bool synchronous, const Any &data);
        virtual uint32 getStrategyId() { return BY_DISTANCE; }

    protected:
        void autoUpdateLodByDistance(Terrain *terrain, bool synchronous, const Real holdDistance);
        int traverseTreeByDistance(TerrainQuadTreeNode *node, const Camera *cam, Real cFactor, const Real holdDistance);
    };

    class _OgreTerrainExport TerrainAutoUpdateLodFactory
    {
    public:
        static TerrainAutoUpdateLod* getAutoUpdateLod(uint32 strategy)
        {
            switch (strategy)
            {
            case BY_DISTANCE:
                return OGRE_NEW TerrainAutoUpdateLodByDistance;
            case NONE:
            default:
                return 0;
            }
            return 0;
        }
    };
}

#endif