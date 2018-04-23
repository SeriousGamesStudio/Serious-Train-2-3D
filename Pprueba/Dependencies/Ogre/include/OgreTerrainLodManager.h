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

 #ifndef __Ogre_TerrainLodManager_H__
 #define __Ogre_TerrainLodManager_H__

 #include "OgreTerrainPrerequisites.h"
 #include "OgreWorkQueue.h"


 namespace Ogre
 {
     class Terrain;
     class _OgreTerrainExport TerrainLodManager : public WorkQueue::RequestHandler, public WorkQueue::ResponseHandler, public TerrainAlloc
     {
     public:
         static const uint32 TERRAINLODDATA_CHUNK_ID;
         static const uint16 TERRAINLODDATA_CHUNK_VERSION;
         typedef vector<float>::type LodData;
         typedef vector<LodData>::type LodsData;

         struct LoadLodRequest
         {
             LoadLodRequest(TerrainLodManager* r, uint16 preparedLod, uint16 loadedLod, uint16 target)
                 : requestee(r)
, currentPreparedLod(preparedLod)
, currentLoadedLod(loadedLod)
, requestedLod(target)
             {
             }
             TerrainLodManager* requestee;
             uint16 currentPreparedLod;
             uint16 currentLoadedLod;
             uint16 requestedLod;
             _OgreTerrainExport friend std::ostream& operator<<(std::ostream& o, const LoadLodRequest& r)
             { return o; }
         };

         struct LodInfo
         {
             uint treeStart;
             uint treeEnd;
             bool isLast;
             uint16 resolution;
             uint size;
         };
     public:
         TerrainLodManager(Terrain* t, DataStreamPtr& stream);
         TerrainLodManager(Terrain* t, const String& filename = "");
         virtual ~TerrainLodManager();

         static const uint16 WORKQUEUE_LOAD_LOD_DATA_REQUEST;
         virtual bool canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
         virtual bool canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
         virtual WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
         virtual void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);

         void updateToLodLevel(int lodLevel, bool synchronous = false);
         static void saveLodData(StreamSerialiser& stream, Terrain* terrain);

         void fillBufferAtLod(uint lodLevel, const float* data, uint dataSize);
         void readLodData(uint16 lowerLodBound, uint16 higherLodBound);
         void waitForDerivedProcesses();

         int getHighestLodPrepared() { return mHighestLodPrepared; }
         int getHighestLodLoaded() { return mHighestLodLoaded; }
         int getTargetLodLevel() { return mTargetLodLevel; }

         LodInfo& getLodInfo(uint lodLevel)
         {
             if (!mLodInfoTable)
                 buildLodInfoTable();
             return mLodInfoTable[lodLevel];
         }
     private:
         void init();
         void buildLodInfoTable();

         static void separateData(float* data, uint16 size, uint16 numLodLevels, LodsData& lods);
     private:
         Terrain* mTerrain;
         DataStreamPtr mDataStream;
         size_t mStreamOffset;
         uint16 mWorkQueueChannel;

         LodInfo* mLodInfoTable;
         int mTargetLodLevel;
         int mHighestLodPrepared;
         int mHighestLodLoaded;

         bool mIncreaseLodLevelInProgress;
         bool mLastRequestSynchronous;
     };
 }

 #endif
