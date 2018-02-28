/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

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

#include "OgreGLTexture.h"
#include "OgreGLRenderSystem.h"
#include "OgreGLPixelFormat.h"
#include "OgreGLHardwarePixelBuffer.h"

#include "OgreTextureManager.h"
#include "OgreImage.h"
#include "OgreLogManager.h"
#include "OgreCamera.h"
#include "OgreException.h"
#include "OgreRoot.h"
#include "OgreCodec.h"
#include "OgreImageCodec.h"
#include "OgreStringConverter.h"
#include "OgreBitwise.h"

#include "OgreGLFBORenderTexture.h"
#include "OgreGLStateCacheManager.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#  define WIN32_LEAN_AND_MEAN
#  if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX // required to stop windows.h messing up std::min
#  endif
#  include <windows.h>
#  include <wingdi.h>
#endif

namespace Ogre {



    GLTexture::GLTexture(ResourceManager* creator, const String& name, 
        ResourceHandle handle, const String& group, bool isManual, 
        ManualResourceLoader* loader, GLRenderSystem* renderSystem) 
        : GLTextureCommon(creator, name, handle, group, isManual, loader),
          mRenderSystem(renderSystem)
    {
    }


    GLTexture::~GLTexture()
    {
        // have to call this here rather than in Resource destructor
        // since calling virtual methods in base destructors causes crash
        if (isLoaded())
        {
            unload(); 
        }
        else
        {
            freeInternalResources();
        }
    }

    GLenum GLTexture::getGLTextureTarget(void) const
    {
        switch(mTextureType)
        {
            case TEX_TYPE_1D:
                return GL_TEXTURE_1D;
            case TEX_TYPE_2D:
                return GL_TEXTURE_2D;
            case TEX_TYPE_3D:
                return GL_TEXTURE_3D;
            case TEX_TYPE_CUBE_MAP:
                return GL_TEXTURE_CUBE_MAP;
            case TEX_TYPE_2D_ARRAY:
                return GL_TEXTURE_2D_ARRAY_EXT;
            default:
                return 0;
        };
    }

    //* Creation / loading methods ********************************************
    void GLTexture::createInternalResourcesImpl(void)
    {
        if (!GLEW_VERSION_1_2 && mTextureType == TEX_TYPE_3D)
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, 
                "3D Textures not supported before OpenGL 1.2", 
                "GLTexture::createInternalResourcesImpl");

        if (!GLEW_VERSION_2_0 && mTextureType == TEX_TYPE_2D_ARRAY)
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, 
                "2D texture arrays not supported before OpenGL 2.0", 
                "GLTexture::createInternalResourcesImpl");

        // Convert to nearest power-of-two size if required
        mWidth = GLPixelUtil::optionalPO2(mWidth);      
        mHeight = GLPixelUtil::optionalPO2(mHeight);
        mDepth = GLPixelUtil::optionalPO2(mDepth);
        

        // Adjust format if required
        mFormat = TextureManager::getSingleton().getNativeFormat(mTextureType, mFormat, mUsage);
        
        // Check requested number of mipmaps
        uint32 maxMips = getMaxMipmaps();
        mNumMipmaps = mNumRequestedMipmaps;
        if(mNumMipmaps>maxMips)
            mNumMipmaps = maxMips;

        // Check if we can do HW mipmap generation
        mMipmapsHardwareGenerated =
            Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_AUTOMIPMAP);
        
        // Generate texture name
        glGenTextures( 1, &mTextureID );
        
        // Set texture type
        mRenderSystem->_getStateCacheManager()->bindGLTexture( getGLTextureTarget(), mTextureID );
        
        // This needs to be set otherwise the texture doesn't get rendered
        if (GLEW_VERSION_1_2)
            mRenderSystem->_getStateCacheManager()->setTexParameteri(getGLTextureTarget(),
                GL_TEXTURE_MAX_LEVEL, mNumMipmaps);
        
        // Set some misc default parameters so NVidia won't complain, these can of course be changed later
        mRenderSystem->_getStateCacheManager()->setTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        mRenderSystem->_getStateCacheManager()->setTexParameteri(getGLTextureTarget(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if (GLEW_VERSION_1_2)
        {
            mRenderSystem->_getStateCacheManager()->setTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            mRenderSystem->_getStateCacheManager()->setTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        if((mUsage & TU_AUTOMIPMAP) &&
            mNumRequestedMipmaps && mMipmapsHardwareGenerated)
        {
            mRenderSystem->_getStateCacheManager()->setTexParameteri( getGLTextureTarget(), GL_GENERATE_MIPMAP, GL_TRUE );
        }
        
        // Allocate internal buffer so that glTexSubImageXD can be used
        // Internal format
        GLenum internalformat = GLPixelUtil::getGLInternalFormat(mFormat, mHwGamma);
        uint32 width = mWidth;
        uint32 height = mHeight;
        uint32 depth = mDepth;

        GLenum format = GLPixelUtil::getGLOriginFormat(mFormat);
        GLenum datatype = GLPixelUtil::getGLOriginDataType(mFormat);

        if(PixelUtil::isCompressed(mFormat))
        {
            // Compressed formats
            GLsizei size = static_cast<GLsizei>(PixelUtil::getMemorySize(mWidth, mHeight, mDepth, mFormat));
            // Provide temporary buffer filled with zeroes as glCompressedTexImageXD does not
            // accept a 0 pointer like normal glTexImageXD
            // Run through this process for every mipmap to pregenerate mipmap piramid
            vector<uint8>::type tmpdata(size);
            
            for(uint32 mip=0; mip<=mNumMipmaps; mip++)
            {
                size = static_cast<GLsizei>(PixelUtil::getMemorySize(width, height, depth, mFormat));
                switch(mTextureType)
                {
                    case TEX_TYPE_1D:
                        glCompressedTexImage1DARB(GL_TEXTURE_1D, mip, internalformat, 
                            width, 0, 
                            size, &tmpdata[0]);
                        break;
                    case TEX_TYPE_2D:
                        glCompressedTexImage2DARB(GL_TEXTURE_2D, mip, internalformat,
                            width, height, 0, 
                            size, &tmpdata[0]);
                        break;
                    case TEX_TYPE_2D_ARRAY:
                    case TEX_TYPE_3D:
                        glCompressedTexImage3DARB(getGLTextureTarget(), mip, internalformat,
                            width, height, depth, 0, 
                            size, &tmpdata[0]);
                        break;
                    case TEX_TYPE_CUBE_MAP:
                        for(int face=0; face<6; face++) {
                            glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, internalformat,
                                width, height, 0, 
                                size, &tmpdata[0]);
                        }
                        break;
                    case TEX_TYPE_2D_RECT:
                        break;
                };
                if(width>1)
                    width = width/2;
                if(height>1)
                    height = height/2;
                if(depth>1 && mTextureType != TEX_TYPE_2D_ARRAY)
                    depth = depth/2;
            }
        }
        else
        {
            // Run through this process to pregenerate mipmap pyramid
            for(uint32 mip=0; mip<=mNumMipmaps; mip++)
            {
                // Normal formats
                switch(mTextureType)
                {
                    case TEX_TYPE_1D:
                        glTexImage1D(GL_TEXTURE_1D, mip, internalformat,
                            width, 0, 
                            format, datatype, 0);
    
                        break;
                    case TEX_TYPE_2D:
                        glTexImage2D(GL_TEXTURE_2D, mip, internalformat,
                            width, height, 0, 
                            format, datatype, 0);
                        break;
                    case TEX_TYPE_2D_ARRAY:
                    case TEX_TYPE_3D:
                        glTexImage3D(getGLTextureTarget(), mip, internalformat,
                            width, height, depth, 0, 
                            format, datatype, 0);
                        break;
                    case TEX_TYPE_CUBE_MAP:
                        for(int face=0; face<6; face++) {
                            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, internalformat,
                                width, height, 0, 
                                format, datatype, 0);
                        }
                        break;
                    case TEX_TYPE_2D_RECT:
                        break;
                };
                if(width>1)
                    width = width/2;
                if(height>1)
                    height = height/2;
                if(depth>1 && mTextureType != TEX_TYPE_2D_ARRAY)
                    depth = depth/2;
            }
        }
        _createSurfaceList();
        // Get final internal format
        mFormat = getBuffer(0,0)->getFormat();
    }
    
    void GLTexture::loadImpl()
    {
        if( mUsage & TU_RENDERTARGET )
        {
            createRenderTexture();
            return;
        }

        LoadedImages loadedImages;
        // Now the only copy is on the stack and will be cleaned in case of
        // exceptions being thrown from _loadImages
        std::swap(loadedImages, mLoadedImages);

        // Call internal _loadImages, not loadImage since that's external and 
        // will determine load status etc again
        ConstImagePtrList imagePtrs;
        for (size_t i=0 ; i<loadedImages.size() ; ++i) {
            imagePtrs.push_back(&loadedImages[i]);
        }

        _loadImages(imagePtrs);
    }

    //*************************************************************************
    
    void GLTexture::freeInternalResourcesImpl()
    {
        mSurfaceList.clear();
        if (GLStateCacheManager* stateCacheManager = mRenderSystem->_getStateCacheManager())
        {
            glDeleteTextures(1, &mTextureID);
            stateCacheManager->invalidateStateForTexture(mTextureID);
        }
    }
    
    //---------------------------------------------------------------------------------------------
    void GLTexture::_createSurfaceList()
    {
        mSurfaceList.clear();
        
        uint32 depth = mDepth;

        // For all faces and mipmaps, store surfaces as HardwarePixelBufferSharedPtr
        for(GLint face=0; face<static_cast<GLint>(getNumFaces()); face++)
        {
            uint32 width = mWidth;
            uint32 height = mHeight;

            for(uint32 mip=0; mip<=getNumMipmaps(); mip++)
            {
                GLHardwarePixelBuffer* buf =
                    new GLTextureBuffer(mRenderSystem, this, face, mip, width, height, depth);
                mSurfaceList.push_back(HardwarePixelBufferSharedPtr(buf));
                
                if (width > 1)
                    width = width / 2;
                if (height > 1)
                    height = height / 2;
                if (depth > 1 && mTextureType != TEX_TYPE_2D_ARRAY)
                    depth = depth / 2;
            }
        }
    }
}

