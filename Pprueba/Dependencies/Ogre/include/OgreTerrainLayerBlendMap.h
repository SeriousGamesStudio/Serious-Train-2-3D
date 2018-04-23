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

 #ifndef __Ogre_TerrainLayerBlendMap_H__
 #define __Ogre_TerrainLayerBlendMap_H__

 #include "OgreTerrainPrerequisites.h"
 #include "OgreCommon.h"
 #include "OgreVector3.h"
 #include "OgreDataStream.h"

 namespace Ogre
 {
     class Image;
     class _OgreTerrainExport TerrainLayerBlendMap : public TerrainAlloc
     {
     protected:
         Terrain* mParent;
         uint8 mLayerIdx;
         uint8 mChannel; // RGBA
         uint8 mChannelOffset; // in pixel format
         Box mDirtyBox;
         bool mDirty;
         HardwarePixelBuffer* mBuffer;
         float* mData;

         void download();
         void upload();

     public:
         TerrainLayerBlendMap(Terrain* parent, uint8 layerIndex, HardwarePixelBuffer* buf);
         virtual ~TerrainLayerBlendMap();
         Terrain* getParent() const { return mParent; }
         uint8 getLayerIndex() const { return mLayerIdx; }

         void convertWorldToUVSpace(const Vector3& worldPos, Real *outX, Real* outY);

         void convertUVToWorldSpace(Real x, Real y, Vector3* outWorldPos);

         void convertUVToImageSpace(Real x, Real y, size_t* outX, size_t* outY);
         void convertImageToUVSpace(size_t x, size_t y, Real* outX, Real* outY);
         void convertImageToTerrainSpace(size_t x, size_t y, Real* outX, Real* outY);
         void convertTerrainToImageSpace(Real x, Real y, size_t* outX, size_t* outY);

         float getBlendValue(size_t x, size_t y);

         void setBlendValue(size_t x, size_t y, float val);

         float* getBlendPointer();

         void dirty();

         void dirtyRect(const Rect& rect);

         void blit(const PixelBox &src, const Box &dstBox);

         void blit(const PixelBox &src);

         void loadImage(const Image& img);

         void loadImage(DataStreamPtr& stream, const String& ext = StringUtil::BLANK);

         void loadImage(const String& filename, const String& groupName);

         void update();


     };

     typedef vector<TerrainLayerBlendMap*>::type TerrainLayerBlendMapList;

 }



 #endif