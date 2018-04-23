
/*-----------------------------------------------------------------------------
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

#ifndef __Ogre_TerrainGroup_H__
#define __Ogre_TerrainGroup_H__

#include "OgreTerrainPrerequisites.h"
#include "OgreTerrain.h"
#include "OgreWorkQueue.h"
#include "OgreIteratorWrappers.h"
#include "OgreTerrainAutoUpdateLod.h"

namespace Ogre
{
    class _OgreTerrainExport TerrainGroup : public WorkQueue::RequestHandler,
        public WorkQueue::ResponseHandler, public TerrainAlloc
    {
    public:
        TerrainGroup(SceneManager* sm, Terrain::Alignment align, uint16 terrainSize,
            Real terrainWorldSize);
        TerrainGroup(SceneManager* sm);
        virtual ~TerrainGroup();

         virtual Terrain::ImportData& getDefaultImportSettings() { return mDefaultImportData; }

         virtual void setOrigin(const Vector3& pos);

         virtual const Vector3& getOrigin() const { return mOrigin; }

         virtual Terrain::Alignment getAlignment() const { return mAlignment; }

         virtual Real getTerrainWorldSize() const { return mTerrainWorldSize; }
         virtual void setTerrainWorldSize(Real newWorldSize);
         virtual uint16 getTerrainSize() const { return mTerrainSize; }
         virtual void setTerrainSize(uint16 newTerrainSize);
         virtual SceneManager* getSceneManager() const { return mSceneManager; }

         void setFilenameConvention(const String& prefix, const String& extension);
         void setFilenamePrefix(const String& prefix);
         void setFilenameExtension(const String& extension);
         const String& getFilenamePrefix() const { return mFilenamePrefix; }
         const String& getFilenameExtension() const { return mFilenameExtension; }

         void setResourceGroup(const String& grp) { mResourceGroup = grp; }
         const String& getResourceGroup() const { return mResourceGroup; }
         virtual void defineTerrain(long x, long y);

         virtual void defineTerrain(long x, long y, float constantHeight);

         virtual void defineTerrain(long x, long y, const Terrain::ImportData* importData);

         virtual void defineTerrain(long x, long y, const Image* img, const Terrain::LayerInstanceList* layers = 0);

         virtual void defineTerrain(long x, long y, const float* pFloat, const Terrain::LayerInstanceList* layers = 0);

         virtual void defineTerrain(long x, long y, const String& filename);


         virtual void loadAllTerrains(bool synchronous = false);

         virtual void loadTerrain(long x, long y, bool synchronous = false);

         virtual void unloadTerrain(long x, long y);

         virtual void removeTerrain(long x, long y);

         void removeAllTerrains();

         void saveAllTerrains(bool onlyIfModified, bool replaceManualFilenames = true);

         struct _OgreTerrainExport TerrainSlotDefinition
         {
             String filename;
             Terrain::ImportData* importData;

             TerrainSlotDefinition() :importData(0) {}
             ~TerrainSlotDefinition();

             void useImportData();
             void useFilename();
             void freeImportData();
         };

         struct _OgreTerrainExport TerrainSlot : public TerrainAlloc
         {
             long x, y;
             TerrainSlotDefinition def;
             Terrain* instance;

             TerrainSlot(long _x, long _y) : x(_x), y(_y), instance(0) {}
             virtual ~TerrainSlot();
             void freeInstance();
         };

         virtual TerrainSlotDefinition* getTerrainDefinition(long x, long y) const;

         virtual Terrain* getTerrain(long x, long y) const;

         void freeTemporaryResources();

         void update(bool synchronous = false);

         void updateGeometry();

         void updateDerivedData(bool synchronous = false, uint8 typeMask = 0xFF);

         struct _OgreTerrainExport RayResult
         {
             bool hit;
             Terrain* terrain;
             Vector3 position;

             RayResult(bool _hit, Terrain* _terrain, const Vector3& _pos)
                 : hit(_hit), terrain(_terrain), position(_pos) {}
         };

         float getHeightAtWorldPosition(Real x, Real y, Real z, Terrain** ppTerrain = 0);

         float getHeightAtWorldPosition(const Vector3& pos, Terrain** ppTerrain = 0);

         RayResult rayIntersects(const Ray& ray, Real distanceLimit = 0) const;

         typedef vector<Terrain*>::type TerrainList;
         void boxIntersects(const AxisAlignedBox& box, TerrainList* resultList) const;
         void sphereIntersects(const Sphere& sphere, TerrainList* resultList) const;

         void convertWorldPositionToTerrainSlot(const Vector3& pos, long *x, long *y) const;

         void convertTerrainSlotToWorldPosition(long x, long y, Vector3* pos) const;

         bool isDerivedDataUpdateInProgress() const;

         typedef map<uint32, TerrainSlot*>::type TerrainSlotMap;
         typedef MapIterator<TerrainSlotMap> TerrainIterator;
         typedef ConstMapIterator<TerrainSlotMap> ConstTerrainIterator;

         TerrainIterator getTerrainIterator();
         ConstTerrainIterator getTerrainIterator() const;

         bool canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
         WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
         bool canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
         void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);

         uint32 packIndex(long x, long y) const;

         void unpackIndex(uint32 key, long *x, long *y);

         String generateFilename(long x, long y) const;

         void saveGroupDefinition(const String& filename);
         void saveGroupDefinition(StreamSerialiser& stream);
         void loadGroupDefinition(const String& filename);
         void loadGroupDefinition(StreamSerialiser& stream);


         static const uint16 WORKQUEUE_LOAD_REQUEST;
         static const uint32 CHUNK_ID;
         static const uint16 CHUNK_VERSION;

         void increaseLodLevel(long x, long y, bool synchronous = false);
         void decreaseLodLevel(long x, long y);

         void setAutoUpdateLod(TerrainAutoUpdateLod* updater);
         void autoUpdateLod(long x, long y, bool synchronous, const Any &data);
         void autoUpdateLodAll(bool synchronous, const Any &data);

     protected:
         SceneManager *mSceneManager;
         Terrain::Alignment mAlignment;
         uint16 mTerrainSize;
         Real mTerrainWorldSize;
         Terrain::ImportData mDefaultImportData;
         Vector3 mOrigin;
         TerrainSlotMap mTerrainSlots;
         uint16 mWorkQueueChannel;
         String mFilenamePrefix;
         String mFilenameExtension;
         String mResourceGroup;
         TerrainAutoUpdateLod *mAutoUpdateLod;
         Terrain::DefaultGpuBufferAllocator mBufferAllocator;

         Vector3 getTerrainSlotPosition(long x, long y);
         TerrainSlot* getTerrainSlot(long x, long y, bool createIfMissing);
         TerrainSlot* getTerrainSlot(long x, long y) const;
         void connectNeighbour(TerrainSlot* slot, long offsetx, long offsety);

         void loadTerrainImpl(TerrainSlot* slot, bool synchronous);

         struct LoadRequest
         {
             TerrainSlot* slot;
             TerrainGroup* origin;
             static uint loadingTaskNum;
             _OgreTerrainExport friend std::ostream& operator<<(std::ostream& o, const LoadRequest& r)
             { return o; }
         };


     };


 }

 #endif