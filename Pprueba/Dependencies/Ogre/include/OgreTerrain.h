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

#ifndef __Ogre_Terrain_H__
#define __Ogre_Terrain_H__

#include "OgreTerrainPrerequisites.h"
#include "OgreCommon.h"
#include "OgreVector3.h"
#include "OgreAxisAlignedBox.h"
#include "OgreSceneManager.h"
#include "OgreTerrainMaterialGenerator.h"
#include "OgreTerrainLayerBlendMap.h"
#include "OgreWorkQueue.h"
#include "OgreTerrainLodManager.h"

namespace Ogre
{
    class _OgreTerrainExport Terrain : public SceneManager::Listener,
        public WorkQueue::RequestHandler, public WorkQueue::ResponseHandler, public TerrainAlloc
    {
    public:
        friend class TerrainLodManager;

        Terrain(SceneManager* sm);
        virtual ~Terrain();

        static const uint32 TERRAIN_CHUNK_ID;
        static const uint16 TERRAIN_CHUNK_VERSION;
        static const uint16 TERRAIN_MAX_BATCH_SIZE;
        static const uint64 TERRAIN_GENERATE_MATERIAL_INTERVAL_MS;

        static const uint32 TERRAINLAYERDECLARATION_CHUNK_ID;
        static const uint16 TERRAINLAYERDECLARATION_CHUNK_VERSION;
        static const uint32 TERRAINLAYERSAMPLER_CHUNK_ID;
        static const uint16 TERRAINLAYERSAMPLER_CHUNK_VERSION;
        static const uint32 TERRAINLAYERSAMPLERELEMENT_CHUNK_ID;
        static const uint16 TERRAINLAYERSAMPLERELEMENT_CHUNK_VERSION;
        static const uint32 TERRAINLAYERINSTANCE_CHUNK_ID;
        static const uint16 TERRAINLAYERINSTANCE_CHUNK_VERSION;
        static const uint32 TERRAINDERIVEDDATA_CHUNK_ID;
        static const uint16 TERRAINDERIVEDDATA_CHUNK_VERSION;
        static const uint32 TERRAINGENERALINFO_CHUNK_ID;
        static const uint16 TERRAINGENERALINFO_CHUNK_VERSION;

        static const size_t LOD_MORPH_CUSTOM_PARAM;

        typedef vector<Real>::type RealVector;

        struct _OgreTerrainExport LayerInstance
        {
            Real worldSize;
            StringVector textureNames;

            LayerInstance()
                : worldSize(100) {}
        };
        typedef vector<LayerInstance>::type LayerInstanceList;

        enum Alignment
        {
            ALIGN_X_Z = 0,
            ALIGN_X_Y = 1,
            ALIGN_Y_Z = 2
        };

        struct ImportData
        {
            Alignment terrainAlign;
            uint16 terrainSize;
            uint16 maxBatchSize;
            uint16 minBatchSize;

            Vector3 pos;

            Real worldSize;

            Image* inputImage;

            float* inputFloat;

            float constantHeight;

            bool deleteInputData;

            Real inputScale;
            Real inputBias;

            TerrainLayerDeclaration layerDeclaration;
            LayerInstanceList layerList;

            ImportData()
                : terrainAlign(ALIGN_X_Z)
, terrainSize(1025)
, maxBatchSize(65)
, minBatchSize(17)
, pos(Vector3::ZERO)
, worldSize(1000)
, inputImage(0)
, inputFloat(0)
, constantHeight(0)
, deleteInputData(false)
, inputScale(1.0)
, inputBias(0.0)
            {

            }

            ImportData(const ImportData& rhs)
                : terrainAlign(ALIGN_X_Z)
, terrainSize(1025)
, maxBatchSize(65)
, minBatchSize(17)
, pos(Vector3::ZERO)
, worldSize(1000)
, inputImage(0)
, inputFloat(0)
, constantHeight(0)
, deleteInputData(false)
, inputScale(1.0)
, inputBias(0.0)
            {
* this = rhs;
            }

            ImportData& operator=(const ImportData& rhs)
            {
                // basic copy
                terrainAlign = rhs.terrainAlign;
                terrainSize = rhs.terrainSize;
                maxBatchSize = rhs.maxBatchSize;
                minBatchSize = rhs.minBatchSize;
                pos = rhs.pos;
                worldSize = rhs.worldSize;
                constantHeight = rhs.constantHeight;
                deleteInputData = rhs.deleteInputData;
                inputScale = rhs.inputScale;
                inputBias = rhs.inputBias;
                layerDeclaration = rhs.layerDeclaration;
                layerList = rhs.layerList;

                // By-value copies in ownership cases
                if (rhs.deleteInputData)
                {
                    if (rhs.inputImage)
                        inputImage = OGRE_NEW Image(*rhs.inputImage);
                    else
                        inputImage = 0;

                    if (rhs.inputFloat)
                    {
                        inputFloat = OGRE_ALLOC_T(float, terrainSize*terrainSize, MEMCATEGORY_GEOMETRY);
                        memcpy(inputFloat, rhs.inputFloat, sizeof(float) * terrainSize*terrainSize);
                    }
                    else
                        inputFloat = 0;
                }
                else
                {
                    // re-use pointers
                    inputImage = rhs.inputImage;
                    inputFloat = rhs.inputFloat;
                }
                return *this;
            }

            void destroy()
            {
                if (deleteInputData)
                {
                    OGRE_DELETE inputImage;
                    OGRE_FREE(inputFloat, MEMCATEGORY_GEOMETRY);
                    inputImage = 0;
                    inputFloat = 0;
                }

            }

            ~ImportData()
            {
                destroy();
            }

        };

        enum NeighbourIndex
        {
            NEIGHBOUR_EAST = 0,
            NEIGHBOUR_NORTHEAST = 1,
            NEIGHBOUR_NORTH = 2,
            NEIGHBOUR_NORTHWEST = 3,
            NEIGHBOUR_WEST = 4,
            NEIGHBOUR_SOUTHWEST = 5,
            NEIGHBOUR_SOUTH = 6,
            NEIGHBOUR_SOUTHEAST = 7,

            NEIGHBOUR_COUNT = 8
        };

        SceneManager* getSceneManager() const { return mSceneMgr; }

        enum Space
        {
            WORLD_SPACE = 0,
            LOCAL_SPACE = 1,
            TERRAIN_SPACE = 2,
            POINT_SPACE = 3
        };

        class _OgreTerrainExport GpuBufferAllocator : public TerrainAlloc
        {
        public:
            GpuBufferAllocator() {}
            virtual ~GpuBufferAllocator() {}

            virtual void allocateVertexBuffers(Terrain* forTerrain, size_t numVertices, HardwareVertexBufferSharedPtr& destPos, HardwareVertexBufferSharedPtr& destDelta) = 0;
            virtual void freeVertexBuffers(const HardwareVertexBufferSharedPtr& posbuf, const HardwareVertexBufferSharedPtr& deltabuf) = 0;

            virtual HardwareIndexBufferSharedPtr getSharedIndexBuffer(uint16 batchSize,
                uint16 vdatasize, size_t vertexIncrement, uint16 xoffset, uint16 yoffset, uint16 numSkirtRowsCols,
                uint16 skirtRowColSkip) = 0;

            virtual void freeAllBuffers() = 0;

        };
        class _OgreTerrainExport DefaultGpuBufferAllocator : public GpuBufferAllocator
        {
        public:
            DefaultGpuBufferAllocator();
            virtual ~DefaultGpuBufferAllocator();
            void allocateVertexBuffers(Terrain* forTerrain, size_t numVertices, HardwareVertexBufferSharedPtr& destPos, HardwareVertexBufferSharedPtr& destDelta);
            void freeVertexBuffers(const HardwareVertexBufferSharedPtr& posbuf, const HardwareVertexBufferSharedPtr& deltabuf);
            HardwareIndexBufferSharedPtr getSharedIndexBuffer(uint16 batchSize,
                uint16 vdatasize, size_t vertexIncrement, uint16 xoffset, uint16 yoffset, uint16 numSkirtRowsCols,
                uint16 skirtRowColSkip);
            void freeAllBuffers();

            void warmStart(size_t numInstances, uint16 terrainSize, uint16 maxBatchSize,
                uint16 minBatchSize);

        protected:
            typedef list<HardwareVertexBufferSharedPtr>::type VBufList;
            VBufList mFreePosBufList;
            VBufList mFreeDeltaBufList;
            typedef map<uint32, HardwareIndexBufferSharedPtr>::type IBufMap;
            IBufMap mSharedIBufMap;

            uint32 hashIndexBuffer(uint16 batchSize,
                uint16 vdatasize, size_t vertexIncrement, uint16 xoffset, uint16 yoffset, uint16 numSkirtRowsCols,
                uint16 skirtRowColSkip);
            HardwareVertexBufferSharedPtr getVertexBuffer(VBufList& list, size_t vertexSize, size_t numVertices);

        };

        void setGpuBufferAllocator(GpuBufferAllocator* alloc);

        GpuBufferAllocator* getGpuBufferAllocator();

        static size_t _getNumIndexesForBatchSize(uint16 batchSize);
        static void _populateIndexBuffer(uint16* pIndexes, uint16 batchSize,
            uint16 vdatasize, size_t vertexIncrement, uint16 xoffset, uint16 yoffset, uint16 numSkirtRowsCols,
            uint16 skirtRowColSkip);

        static uint16 _calcSkirtVertexIndex(uint16 mainIndex, uint16 vdatasize, bool isCol,
            uint16 numSkirtRowsCols, uint16 skirtRowColSkip);

        void convertPosition(Space inSpace, const Vector3& inPos, Space outSpace, Vector3& outPos) const;
        Vector3 convertPosition(Space inSpace, const Vector3& inPos, Space outSpace) const;
        void convertDirection(Space inSpace, const Vector3& inDir, Space outSpace, Vector3& outDir) const;
        Vector3 convertDirection(Space inSpace, const Vector3& inDir, Space outSpace) const;

        void setResourceGroup(const String& resGroup) { mResourceGroup = resGroup; }

        const String& getResourceGroup() const { return mResourceGroup; }

        const String& _getDerivedResourceGroup() const;

        void save(const String& filename);
        void save(StreamSerialiser& stream);

        bool prepare(const String& filename);
        bool prepare(DataStreamPtr& stream);
        bool prepare(StreamSerialiser& stream);

        bool prepare(const ImportData& importData);

        void load(const String& filename);

        void load(StreamSerialiser& stream);

        void load(int lodLevel = 0, bool synchronous = true);

        bool isLoaded() const { return mIsLoaded; }

        bool isModified() const { return mModified; }


        bool isHeightDataModified() const { return mHeightDataModified; }


        void unload();

        void unprepare();


        float* getHeightData() const;

        float* getHeightData(long x, long y) const;

        float getHeightAtPoint(long x, long y) const;

        void setHeightAtPoint(long x, long y, float h);

        float getHeightAtTerrainPosition(Real x, Real y) const;

        float getHeightAtWorldPosition(Real x, Real y, Real z) const;

        float getHeightAtWorldPosition(const Vector3& pos) const;

        const float* getDeltaData() const;

        const float* getDeltaData(long x, long y) const;

        void getPoint(long x, long y, Vector3* outpos) const;

        void getPointFromSelfOrNeighbour(long x, long y, Vector3* outpos) const;

        void getPoint(long x, long y, float height, Vector3* outpos) const;
        void getPointTransform(Matrix4* outXform) const;
        void getTerrainVector(const Vector3& inVec, Vector3* outVec) const;
        void getTerrainVectorAlign(const Vector3& inVec, Alignment align, Vector3* outVec) const;

        void getTerrainVector(Real x, Real y, Real z, Vector3* outVec) const;
        void getTerrainVectorAlign(Real x, Real y, Real z, Alignment align, Vector3* outVec) const;

        void getVector(const Vector3& inVec, Vector3* outVec) const;
        void getVectorAlign(const Vector3& inVec, Alignment align, Vector3* outVec) const;

        void getVector(Real x, Real y, Real z, Vector3* outVec) const;
        void getVectorAlign(Real x, Real y, Real z, Alignment align, Vector3* outVec) const;


        void getPosition(const Vector3& TSpos, Vector3* outWSpos) const;
        void getPosition(Real x, Real y, Real z, Vector3* outWSpos) const;

        void getTerrainPosition(const Vector3& WSpos, Vector3* outTSpos) const;
        void getTerrainPosition(Real x, Real y, Real z, Vector3* outTSpos) const;
        void getPositionAlign(const Vector3& TSpos, Alignment align, Vector3* outWSpos) const;
        void getPositionAlign(Real x, Real y, Real z, Alignment align, Vector3* outWSpos) const;

        void getTerrainPositionAlign(const Vector3& WSpos, Alignment align, Vector3* outTSpos) const;
        void getTerrainPositionAlign(Real x, Real y, Real z, Alignment align, Vector3* outTSpos) const;


        Alignment getAlignment() const;
        uint16 getSize() const;
        void setSize(uint16 newSize);
        uint16 getMaxBatchSize() const;
        uint16 getMinBatchSize() const;
        Real getWorldSize() const;
        void setWorldSize(Real newWorldSize);

        uint8 getLayerCount() const { return static_cast<uint8>(mLayers.size()); }

        const TerrainLayerDeclaration& getLayerDeclaration() const { return mLayerDecl; }

        void addLayer(Real worldSize = 0, const StringVector* textureNames = 0);

        void addLayer(uint8 index, Real worldSize = 0, const StringVector* textureNames = 0);

        void removeLayer(uint8 index);

        void replaceLayer(uint8 index, bool keepBlends, Real worldSize = 0, const StringVector* textureNames = 0);

        uint8 getMaxLayers() const;

        Real getLayerWorldSize(uint8 index) const;
        void setLayerWorldSize(uint8 index, Real size);

        Real getLayerUVMultiplier(uint8 index) const;

        const String& getLayerTextureName(uint8 layerIndex, uint8 samplerIndex) const;
        void setLayerTextureName(uint8 layerIndex, uint8 samplerIndex, const String& textureName);

        uint16 getLayerBlendMapSize() const { return mLayerBlendMapSize; }

        uint16 getLightmapSize() const { return mLightmapSize; }

        const TexturePtr& getLightmap() const { return mLightmap; }

        uint16 getCompositeMapSize() const { return mCompositeMapSize; }

        const TexturePtr& getCompositeMap() const { return mCompositeMap; }

        const Vector3& getPosition() const { return mPos; }
        void setPosition(const Vector3& pos);
        SceneNode* _getRootSceneNode() const;
        void dirty();

        void dirtyRect(const Rect& rect);

        void _dirtyCompositeMapRect(const Rect& rect);

        void dirtyLightmapRect(const Rect& rect);

        void dirtyLightmap();

        void update(bool synchronous = false);

        void updateGeometry();
        void updateGeometryWithoutNotifyNeighbours();

        // Used as a type mask for updateDerivedData
        static const uint8 DERIVED_DATA_DELTAS;
        static const uint8 DERIVED_DATA_NORMALS;
        static const uint8 DERIVED_DATA_LIGHTMAP;
        static const uint8 DERIVED_DATA_ALL;

        void updateDerivedData(bool synchronous = false, uint8 typeMask = 0xFF);

        void updateCompositeMap();

        void updateCompositeMapWithDelay(Real delay = 2);


        Real getSkirtSize() const { return mSkirtSize; }

        uint16 getNumLodLevels() const { return mNumLodLevels; }

        uint16 getNumLodLevelsPerLeaf() const { return mNumLodLevelsPerLeafNode; }

        Rect calculateHeightDeltas(const Rect& rect);

        void finaliseHeightDeltas(const Rect& rect, bool cpuData);

        PixelBox* calculateNormals(const Rect& rect, Rect& outFinalRect);

        void finaliseNormals(const Rect& rect, PixelBox* normalsBox);

        PixelBox* calculateLightmap(const Rect& rect, const Rect& extraTargetRect, Rect& outFinalRect);

        void finaliseLightmap(const Rect& rect, PixelBox* lightmapBox);

        uint16 getResolutionAtLod(uint16 lodLevel) const;

        std::pair<bool, Vector3> rayIntersects(const Ray& ray,
            bool cascadeToNeighbours = false, Real distanceLimit = 0); //const;

        const AxisAlignedBox& getAABB() const;
        AxisAlignedBox getWorldAABB() const;
        Real getMinHeight() const;
        Real getMaxHeight() const;
        Real getBoundingRadius() const;

        const MaterialPtr& getMaterial() const;
        const MaterialPtr& _getMaterial() const { return mMaterial; }
        const MaterialPtr& getCompositeMapMaterial() const;
        const MaterialPtr& _getCompositeMapMaterial() const { return mCompositeMapMaterial; }

        const String& getMaterialName() const { return mMaterialName; }

        void preFindVisibleObjects(SceneManager* source,
            SceneManager::IlluminationRenderStage irs, Viewport* v);
        void sceneManagerDestroyed(SceneManager* source);

        uint8 getRenderQueueGroup(void) const { return mRenderQueueGroup; }
        void setRenderQueueGroup(uint8 grp) { mRenderQueueGroup = grp; }

        uint32 getVisibilityFlags(void) const { return mVisibilityFlags; }
        void setVisibilityFlags(uint32 flags) { mVisibilityFlags = flags; }

        uint32 getQueryFlags(void) const { return mQueryFlags; }
        void setQueryFlags(uint32 flags) { mQueryFlags = flags; }

        void addQueryFlags(uint32 flags) { mQueryFlags |= flags; }

        /* As setQueryFlags, except the flags passed as parameters are removed from the existing flags on this object. */
        void removeQueryFlags(uint32 flags) { mQueryFlags &= ~flags; }


        TerrainLayerBlendMap* getLayerBlendMap(uint8 layerIndex);

        uint8 getBlendTextureIndex(uint8 layerIndex) const;

        uint8 getBlendTextureCount() const;
        uint8 getBlendTextureCount(uint8 numLayers) const;


        const String& getBlendTextureName(uint8 textureIndex) const;

        void setGlobalColourMapEnabled(bool enabled, uint16 size = 0);
        bool getGlobalColourMapEnabled() const { return mGlobalColourMapEnabled; }
        uint16 getGlobalColourMapSize() const { return mGlobalColourMapSize; }
        const TexturePtr& getGlobalColourMap() const { return mColourMap; }

        void widenRectByVector(const Vector3& vec, const Rect& inRect, Rect& outRect);

        void widenRectByVector(const Vector3& vec, const Rect& inRect,
            Real minHeight, Real maxHeight, Rect& outRect);

        void freeTemporaryResources();

        const TexturePtr& getLayerBlendTexture(uint8 index) const;

        std::pair<uint8, uint8> getLayerBlendTextureIndex(uint8 layerIndex) const;

        void _setMorphRequired(bool morph) { mLodMorphRequired = morph; }
        bool _getMorphRequired() const { return mLodMorphRequired; }

        void _setNormalMapRequired(bool normalMap);

        void _setLightMapRequired(bool lightMap, bool shadowsOnly = false);

        void _setCompositeMapRequired(bool compositeMap);

        bool _getUseVertexCompression() const;

        bool canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
        WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
        bool canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
        void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
        void handleGenerateMaterialResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);

        static const uint16 WORKQUEUE_DERIVED_DATA_REQUEST;
        static const uint16 WORKQUEUE_GENERATE_MATERIAL_REQUEST;

        uint16 getLODLevelWhenVertexEliminated(long x, long y) const;
        uint16 getLODLevelWhenVertexEliminated(long rowOrColulmn) const;


        TerrainQuadTreeNode* getQuadTree() { return mQuadTree; }

        TexturePtr getTerrainNormalMap() const { return mTerrainNormalMap; }

        Terrain* getNeighbour(NeighbourIndex index) const;

        void setNeighbour(NeighbourIndex index, Terrain* neighbour, bool recalculate = false, bool notifyOther = true);

        static NeighbourIndex getOppositeNeighbour(NeighbourIndex index);

        static NeighbourIndex getNeighbourIndex(long offsetx, long offsety);

        void notifyNeighbours();

        void neighbourModified(NeighbourIndex index, const Rect& edgerect, const Rect& shadowrect);

        Terrain* raySelectNeighbour(const Ray& ray, Real distanceLimit = 0);

        void _dumpTextures(const String& prefix, const String& suffix);

        bool isDerivedDataUpdateInProgress() const { return mDerivedDataUpdateInProgress; }


        static void convertWorldToTerrainAxes(Alignment align, const Vector3& worldVec, Vector3* terrainVec);
        static void convertTerrainToWorldAxes(Alignment align, const Vector3& terrainVec, Vector3* worldVec);

        static void writeLayerDeclaration(const TerrainLayerDeclaration& decl, StreamSerialiser& ser);
        static bool readLayerDeclaration(StreamSerialiser& ser, TerrainLayerDeclaration& targetdecl);
        static void writeLayerInstanceList(const Terrain::LayerInstanceList& lst, StreamSerialiser& ser);
        static bool readLayerInstanceList(StreamSerialiser& ser, size_t numSamplers, Terrain::LayerInstanceList& targetlst);

        // This mutex is write-locked by neighbours if they are in the process of deleting themselves.
        // It should be read-locked whenever using neighbours in calculations which are possibly running in a
        // background thread.
        OGRE_RW_MUTEX(mNeighbourMutex);

    protected:
        uint getGeoDataSizeAtLod(uint16 lodLevel) const;
        inline int getPositiveLodLevel(int lodLevel) const
        {
            return (lodLevel >= 0) ? lodLevel : mNumLodLevels + lodLevel;
        }
        void freeLodData();

        void freeCPUResources();
        void freeGPUResources();
        void determineLodLevels();
        void distributeVertexData();
        void updateBaseScale();
        void createGPUBlendTextures();
        void createLayerBlendMaps();
        void createOrDestroyGPUNormalMap();
        void createOrDestroyGPUColourMap();
        void createOrDestroyGPULightmap();
        void createOrDestroyGPUCompositeMap();
        void waitForDerivedProcesses();
        void convertSpace(Space inSpace, const Vector3& inVec, Space outSpace, Vector3& outVec, bool translation) const;
        Vector3 convertWorldToTerrainAxes(const Vector3& inVec) const;
        Vector3 convertTerrainToWorldAxes(const Vector3& inVec) const;
        void getPointAlign(long x, long y, Alignment align, Vector3* outpos) const;
        void getPointAlign(long x, long y, float height, Alignment align, Vector3* outpos) const;
        void calculateCurrentLod(Viewport* vp);
        std::pair<bool, Vector3> checkQuadIntersection(int x, int y, const Ray& ray); //const;

        void deleteBlendMaps(uint8 lowIndex);
        void shiftUpGPUBlendChannels(uint8 index);
        void shiftDownGPUBlendChannels(uint8 index);
        void copyBlendTextureChannel(uint8 srcIndex, uint8 srcChannel, uint8 destIndex, uint8 destChannel);
        void clearGPUBlendChannel(uint8 index, uint channel);

        void copyGlobalOptions();
        void checkLayers(bool includeGPUResources);
        void checkDeclaration();
        void deriveUVMultipliers();
        PixelFormat getBlendTextureFormat(uint8 textureIndex, uint8 numLayers) const;

        void updateDerivedDataImpl(const Rect& rect, const Rect& lightmapExtraRect, bool synchronous, uint8 typeMask);

        void getEdgeRect(NeighbourIndex index, long range, Rect* outRect) const;
        // get the equivalent of the passed in edge rectangle in neighbour
        void getNeighbourEdgeRect(NeighbourIndex index, const Rect& inRect, Rect* outRect) const;
        // get the equivalent of the passed in edge point in neighbour
        void getNeighbourPoint(NeighbourIndex index, long x, long y, long *outx, long *outy) const;
        // overflow a point into a neighbour index and point
        void getNeighbourPointOverflow(long x, long y, NeighbourIndex *outindex, long *outx, long *outy) const;

        void removeFromNeighbours();

        uint16 mWorkQueueChannel;
        SceneManager* mSceneMgr;
        SceneNode* mRootNode;
        String mResourceGroup;
        bool mIsLoaded;
        bool mModified;
        bool mHeightDataModified;

        float* mHeightData;
        float* mDeltaData;
        Alignment mAlign;
        Real mWorldSize;
        uint16 mSize;
        uint16 mMaxBatchSize;
        uint16 mMinBatchSize;
        Vector3 mPos;
        TerrainQuadTreeNode* mQuadTree;
        uint16 mNumLodLevels;
        uint16 mNumLodLevelsPerLeafNode;
        uint16 mTreeDepth;
        Real mBase;
        Real mScale;
        TerrainLayerDeclaration mLayerDecl;
        LayerInstanceList mLayers;
        RealVector mLayerUVMultiplier;

        Real mSkirtSize;
        uint8 mRenderQueueGroup;
        uint32 mVisibilityFlags;
        uint32 mQueryFlags;

        Rect mDirtyGeometryRect;
        Rect mDirtyDerivedDataRect;
        Rect mDirtyGeometryRectForNeighbours;
        Rect mDirtyLightmapFromNeighboursRect;
        bool mDerivedDataUpdateInProgress;
        uint8 mDerivedUpdatePendingMask;

        bool mGenerateMaterialInProgress;
        mutable bool mPrepareInProgress;
        struct DerivedDataRequest
        {
            Terrain* terrain;
            // types requested
            uint8 typeMask;
            Rect dirtyRect;
            Rect lightmapExtraDirtyRect;
            _OgreTerrainExport friend std::ostream& operator<<(std::ostream& o, const DerivedDataRequest& r)
            { return o; }
        };

        struct DerivedDataResponse
        {
            Terrain* terrain;
            uint8 remainingTypeMask;
            Rect deltaUpdateRect;
            Rect normalUpdateRect;
            Rect lightmapUpdateRect;
            PixelBox* normalMapBox;
            PixelBox* lightMapBox;
            _OgreTerrainExport friend std::ostream& operator<<(std::ostream& o, const DerivedDataResponse& r)
            { return o; }
        };

		enum GenerateMaterialStage {
			GEN_MATERIAL,
			GEN_COMPOSITE_MAP_MATERIAL

		};
        struct GenerateMaterialRequest
        {
            Terrain* terrain;
            unsigned long startTime;
            GenerateMaterialStage stage;
            bool synchronous;
            _OgreTerrainExport friend std::ostream& operator<<(std::ostream& o, const GenerateMaterialRequest& r)
            { return o; }
        };

        String mMaterialName;
        mutable MaterialPtr mMaterial;
        mutable TerrainMaterialGeneratorPtr mMaterialGenerator;
        mutable unsigned long long int mMaterialGenerationCount;
        mutable bool mMaterialDirty;
        mutable bool mMaterialParamsDirty;

        uint16 mLayerBlendMapSize;
        uint16 mLayerBlendMapSizeActual;
        typedef vector<uint8*>::type BytePointerList;
        BytePointerList mCpuBlendMapStorage;
        typedef vector<TexturePtr>::type TexturePtrList;
        TexturePtrList mBlendTextureList;
        TerrainLayerBlendMapList mLayerBlendMapList;

        uint16 mGlobalColourMapSize;
        bool mGlobalColourMapEnabled;
        TexturePtr mColourMap;
        uint8* mCpuColourMapStorage;

        uint16 mLightmapSize;
        uint16 mLightmapSizeActual;
        TexturePtr mLightmap;
        uint8* mCpuLightmapStorage;

        uint16 mCompositeMapSize;
        uint16 mCompositeMapSizeActual;
        TexturePtr mCompositeMap;
        uint8* mCpuCompositeMapStorage;
        Rect mCompositeMapDirtyRect;
        unsigned long mCompositeMapUpdateCountdown;
        unsigned long mLastMillis;
        bool mCompositeMapDirtyRectLightmapUpdate;
        mutable MaterialPtr mCompositeMapMaterial;


        static NameGenerator msBlendTextureGenerator;
        static NameGenerator msNormalMapNameGenerator;
        static NameGenerator msLightmapNameGenerator;
        static NameGenerator msCompositeMapNameGenerator;

        bool mLodMorphRequired;
        bool mNormalMapRequired;
        bool mLightMapRequired;
        bool mLightMapShadowsOnly;
        bool mCompositeMapRequired;
        TexturePtr mTerrainNormalMap;

        PixelBox* mCpuTerrainNormalMap;

        const Camera* mLastLODCamera;
        unsigned long mLastLODFrame;
        int mLastViewportHeight;

        Terrain* mNeighbours[NEIGHBOUR_COUNT];

        GpuBufferAllocator* mCustomGpuBufferAllocator;
        DefaultGpuBufferAllocator mDefaultGpuBufferAllocator;

        size_t getPositionBufVertexSize() const;
        size_t getDeltaBufVertexSize() const;

        TerrainLodManager* mLodManager;

    public:
        void increaseLodLevel(bool synchronous = false);
        void decreaseLodLevel();

        int getHighestLodPrepared() const { return (mLodManager) ? mLodManager->getHighestLodPrepared() : -1; };
        int getHighestLodLoaded() const { return (mLodManager) ? mLodManager->getHighestLodLoaded() : -1; };
        int getTargetLodLevel() const { return (mLodManager) ? mLodManager->getTargetLodLevel() : -1; };
    };


    class _OgreTerrainExport TerrainGlobalOptions : public TerrainAlloc, public Singleton<TerrainGlobalOptions>
    {
    protected:

        Real mSkirtSize;
        Vector3 mLightMapDir;
        bool mCastsShadows;
        Real mMaxPixelError;
        uint8 mRenderQueueGroup;
        uint32 mVisibilityFlags;
        uint32 mQueryFlags;
        bool mUseRayBoxDistanceCalculation;
        TerrainMaterialGeneratorPtr mDefaultMaterialGenerator;
        uint16 mLayerBlendMapSize;
        Real mDefaultLayerTextureWorldSize;
        uint16 mDefaultGlobalColourMapSize;
        uint16 mLightmapSize;
        uint16 mCompositeMapSize;
        ColourValue mCompositeMapAmbient;
        ColourValue mCompositeMapDiffuse;
        Real mCompositeMapDistance;
        String mResourceGroup;
        bool mUseVertexCompressionWhenAvailable;

    public:
        TerrainGlobalOptions();
        virtual ~TerrainGlobalOptions() {}


        Real getSkirtSize() const { return mSkirtSize; }
        void setSkirtSize(Real skirtSz) { mSkirtSize = skirtSz; }
        const Vector3& getLightMapDirection() const { return mLightMapDir; }
        void setLightMapDirection(const Vector3& v) { mLightMapDir = v; }
        const ColourValue& getCompositeMapAmbient() const { return mCompositeMapAmbient; }
        void setCompositeMapAmbient(const ColourValue& c) { mCompositeMapAmbient = c; }
        const ColourValue& getCompositeMapDiffuse() const { return mCompositeMapDiffuse; }
        void setCompositeMapDiffuse(const ColourValue& c) { mCompositeMapDiffuse = c; }
        Real getCompositeMapDistance() const { return mCompositeMapDistance; }
        void setCompositeMapDistance(Real c) { mCompositeMapDistance = c; }


        bool getCastsDynamicShadows() const { return mCastsShadows; }

        void setCastsDynamicShadows(bool s) { mCastsShadows = s; }

        Real getMaxPixelError() const { return mMaxPixelError; }

        void setMaxPixelError(Real pixerr) { mMaxPixelError = pixerr; }

        uint8 getRenderQueueGroup(void) const { return mRenderQueueGroup; }
        void setRenderQueueGroup(uint8 grp) { mRenderQueueGroup = grp; }

        uint32 getVisibilityFlags(void) const { return mVisibilityFlags; }
        void setVisibilityFlags(uint32 flags) { mVisibilityFlags = flags; }

        void  setQueryFlags(uint32 flags) { mQueryFlags = flags; }
        uint32 getQueryFlags(void) const { return mQueryFlags; }

        void addQueryFlags(uint32 flags) { mQueryFlags |= flags; }

        /* As setQueryFlags, except the flags passed as parameters are removed from the existing flags on this object. */
        void removeQueryFlags(uint32 flags) { mQueryFlags &= ~flags; }

        bool getUseRayBoxDistanceCalculation() const { return mUseRayBoxDistanceCalculation; }

        void setUseRayBoxDistanceCalculation(bool rb) { mUseRayBoxDistanceCalculation = rb; }

        TerrainMaterialGeneratorPtr getDefaultMaterialGenerator();

        void setDefaultMaterialGenerator(TerrainMaterialGeneratorPtr gen);

        uint16 getLayerBlendMapSize() const { return mLayerBlendMapSize; }

        void setLayerBlendMapSize(uint16 sz) { mLayerBlendMapSize = sz; }

        Real getDefaultLayerTextureWorldSize() const { return mDefaultLayerTextureWorldSize; }

        void setDefaultLayerTextureWorldSize(Real sz) { mDefaultLayerTextureWorldSize = sz; }

        uint16 getDefaultGlobalColourMapSize() const { return mDefaultGlobalColourMapSize; }

        void setDefaultGlobalColourMapSize(uint16 sz) { mDefaultGlobalColourMapSize = sz; }


        uint16 getLightMapSize() const { return mLightmapSize; }

        void setLightMapSize(uint16 sz) { mLightmapSize = sz; }

        uint16 getCompositeMapSize() const { return mCompositeMapSize; }

        void setCompositeMapSize(uint16 sz) { mCompositeMapSize = sz; }

        void setDefaultResourceGroup(const String& grp) { mResourceGroup = grp; }

        const String& getDefaultResourceGroup() const { return mResourceGroup; }

        bool getUseVertexCompressionWhenAvailable() const { return mUseVertexCompressionWhenAvailable; }

        void setUseVertexCompressionWhenAvailable(bool enable) { mUseVertexCompressionWhenAvailable = enable; }

        static TerrainGlobalOptions& getSingleton(void);
        static TerrainGlobalOptions* getSingletonPtr(void);


    };


}




#endif