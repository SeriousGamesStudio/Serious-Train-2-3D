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

#include "OgreGLES2Texture.h"
#include "OgreGLES2PixelFormat.h"
#include "OgreGLES2RenderSystem.h"
#include "OgreGLES2HardwarePixelBuffer.h"
#include "OgreGLES2Support.h"
#include "OgreGLES2StateCacheManager.h"
#include "OgreRoot.h"
#include "OgreBitwise.h"
#include "OgreTextureManager.h"

namespace Ogre {
    GLES2Texture::GLES2Texture(ResourceManager* creator, const String& name,
                             ResourceHandle handle, const String& group, bool isManual,
                             ManualResourceLoader* loader, GLES2RenderSystem* renderSystem)
        : GLTextureCommon(creator, name, handle, group, isManual, loader),
          mRenderSystem(renderSystem)
    {
    }

    GLES2Texture::~GLES2Texture()
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

    GLenum GLES2Texture::getGLES2TextureTarget(void) const
    {
        switch(mTextureType)
        {
            case TEX_TYPE_1D:
            case TEX_TYPE_2D:
                return GL_TEXTURE_2D;
            case TEX_TYPE_CUBE_MAP:
                return GL_TEXTURE_CUBE_MAP;
            case TEX_TYPE_3D:
                return GL_TEXTURE_3D_OES;
            case TEX_TYPE_2D_ARRAY:
                return GL_TEXTURE_2D_ARRAY;
            default:
                return 0;
        };
    }

    void GLES2Texture::_createGLTexResource()
    {
        const RenderSystemCapabilities *renderCaps =
                Root::getSingleton().getRenderSystem()->getCapabilities();

        const bool nonPowerOfTwoSupported = renderCaps->hasCapability(RSC_NON_POWER_OF_2_TEXTURES) ||
                                            ( renderCaps->getNonPOW2TexturesLimited() &&
                                              mNumRequestedMipmaps == 0 );

        if( !nonPowerOfTwoSupported )
        {
            // Convert to nearest power-of-two size if required
            mWidth = Bitwise::firstPO2From(mWidth);
            mHeight = Bitwise::firstPO2From(mHeight);
            mDepth = Bitwise::firstPO2From(mDepth);
        }

        // set HardwareBuffer::Usage for TU_RENDERTARGET if nothing else specified
        if((mUsage & TU_RENDERTARGET) && (mUsage & ~TU_RENDERTARGET) == 0)
            mUsage |= HardwareBuffer::HBU_DYNAMIC;

        // Adjust format if required
        mFormat = TextureManager::getSingleton().getNativeFormat(mTextureType, mFormat, mUsage);
        GLenum texTarget = getGLES2TextureTarget();

        // Check requested number of mipmaps
        uint32 maxMips = getMaxMipmaps();
        
        if(PixelUtil::isCompressed(mFormat) && (mNumMipmaps == 0))
            mNumRequestedMipmaps = 0;
        
        mNumMipmaps = mNumRequestedMipmaps;
        if (mNumMipmaps > maxMips)
            mNumMipmaps = maxMips;

        // Generate texture name
        OGRE_CHECK_GL_ERROR(glGenTextures(1, &mTextureID));
           
        // Set texture type
        mRenderSystem->_getStateCacheManager()->bindGLTexture(texTarget, mTextureID);
        
        // If we can do automip generation and the user desires this, do so
        mMipmapsHardwareGenerated = !PixelUtil::isCompressed(mFormat);

        // glGenerateMipmap require all mip levels to be prepared. So override how many this texture has.
        if((mUsage & TU_AUTOMIPMAP) && mMipmapsHardwareGenerated && mNumRequestedMipmaps)
            mNumMipmaps = maxMips;

        if(mRenderSystem->hasMinGLVersion(3, 0) || mRenderSystem->checkExtension("GL_APPLE_texture_max_level"))
            mRenderSystem->_getStateCacheManager()->setTexParameteri(texTarget, GL_TEXTURE_MAX_LEVEL_APPLE, mNumRequestedMipmaps ? mNumMipmaps + 1 : 0);

        // Set some misc default parameters, these can of course be changed later
        mRenderSystem->_getStateCacheManager()->setTexParameteri(texTarget,
                                                            GL_TEXTURE_MIN_FILTER, ((mUsage & TU_AUTOMIPMAP) && mNumRequestedMipmaps) ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
        mRenderSystem->_getStateCacheManager()->setTexParameteri(texTarget,
                                                            GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        mRenderSystem->_getStateCacheManager()->setTexParameteri(texTarget,
                                                            GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        mRenderSystem->_getStateCacheManager()->setTexParameteri(texTarget,
                                                            GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        bool hasGLES30 = mRenderSystem->hasMinGLVersion(3, 0);
#if OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
        hasGLES30 = false; // still just Editors Draft
#endif

        // Set up texture swizzling
        if (hasGLES30 && PixelUtil::isLuminance(mFormat))
        {
            if (PixelUtil::getComponentCount(mFormat) == 2)
            {
                OGRE_CHECK_GL_ERROR(glTexParameteri(texTarget, GL_TEXTURE_SWIZZLE_R, GL_RED));
                OGRE_CHECK_GL_ERROR(glTexParameteri(texTarget, GL_TEXTURE_SWIZZLE_G, GL_RED));
                OGRE_CHECK_GL_ERROR(glTexParameteri(texTarget, GL_TEXTURE_SWIZZLE_B, GL_RED));
                OGRE_CHECK_GL_ERROR(glTexParameteri(texTarget, GL_TEXTURE_SWIZZLE_A, GL_GREEN));
            }
            else
            {
                OGRE_CHECK_GL_ERROR(glTexParameteri(texTarget, GL_TEXTURE_SWIZZLE_R, GL_RED));
                OGRE_CHECK_GL_ERROR(glTexParameteri(texTarget, GL_TEXTURE_SWIZZLE_G, GL_RED));
                OGRE_CHECK_GL_ERROR(glTexParameteri(texTarget, GL_TEXTURE_SWIZZLE_B, GL_RED));
                OGRE_CHECK_GL_ERROR(glTexParameteri(texTarget, GL_TEXTURE_SWIZZLE_A, GL_ONE));
            }
        }

        // Allocate internal buffer so that glTexSubImageXD can be used
        // Internal format
        GLenum format = GLES2PixelUtil::getGLOriginFormat(mFormat);
        GLenum internalformat = GLES2PixelUtil::getGLInternalFormat(mFormat, mHwGamma);
        uint32 width = mWidth;
        uint32 height = mHeight;
        uint32 depth = mDepth;
        
        if (PixelUtil::isCompressed(mFormat))
        {
            // Compressed formats
            GLsizei size = static_cast<GLsizei>(PixelUtil::getMemorySize(mWidth, mHeight, mDepth, mFormat));
            
            // Provide temporary buffer filled with zeroes as glCompressedTexImageXD does not
            // accept a 0 pointer like normal glTexImageXD
            // Run through this process for every mipmap to pregenerate mipmap pyramid
            
            vector<uint8>::type tmpdata(size);
            for (uint32 mip = 0; mip <= mNumMipmaps; mip++)
            {
#if OGRE_DEBUG_MODE
                LogManager::getSingleton().logMessage("GLES2Texture::create - Mip: " + StringConverter::toString(mip) +
                                                      " Width: " + StringConverter::toString(width) +
                                                      " Height: " + StringConverter::toString(height) +
                                                      " Internal Format: " + StringConverter::toString(internalformat, 0, ' ', std::ios::hex) +
                                                      " Format: " + StringConverter::toString(format, 0, ' ', std::ios::hex)
                                                      );
#endif
                size = static_cast<GLsizei>(PixelUtil::getMemorySize(width, height, depth, mFormat));
                
                switch(mTextureType)
                {
                    case TEX_TYPE_1D:
                    case TEX_TYPE_2D:
                    case TEX_TYPE_2D_RECT:
                        OGRE_CHECK_GL_ERROR(glCompressedTexImage2D(GL_TEXTURE_2D,
                                               mip,
                                               internalformat,
                                               width, height,
                                               0,
                                               size,
                                               &tmpdata[0]));
                        break;
                    case TEX_TYPE_CUBE_MAP:
                        for(int face = 0; face < 6; face++) {
                            OGRE_CHECK_GL_ERROR(glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, internalformat,
                                width, height, 0, 
                                size, &tmpdata[0]));
                        }
                        break;
                    case TEX_TYPE_2D_ARRAY:
                        if(!hasGLES30)
                            break;
                        /* no break */
                    case TEX_TYPE_3D:
                        glCompressedTexImage3DOES(getGLES2TextureTarget(), mip, format,
                            width, height, depth, 0, 
                            size, &tmpdata[0]);
                        break;
                };
                
                if(width > 1)
                {
                    width = width / 2;
                }
                if(height > 1)
                {
                    height = height / 2;
                }
                if(depth > 1 && mTextureType != TEX_TYPE_2D_ARRAY)
                {
                    depth = depth / 2;
                }
            }
            return;
        }

        if(hasGLES30)
        {
#if OGRE_DEBUG_MODE
            LogManager::getSingleton().logMessage("GLES2Texture::create - Name: " + mName +
                                                      " ID: " + StringConverter::toString(mTextureID) +
                                                      " Width: " + StringConverter::toString(width) +
                                                      " Height: " + StringConverter::toString(height) +
                                                      " Internal Format: " + StringConverter::toString(internalformat, 0, ' ', std::ios::hex));
#endif
            switch(mTextureType)
            {
                case TEX_TYPE_1D:
                case TEX_TYPE_2D:
                case TEX_TYPE_2D_RECT:
                    OGRE_CHECK_GL_ERROR(glTexStorage2D(GL_TEXTURE_2D, GLsizei(mNumMipmaps+1), internalformat, GLsizei(width), GLsizei(height)));
                    break;
                case TEX_TYPE_CUBE_MAP:
                    OGRE_CHECK_GL_ERROR(glTexStorage2D(GL_TEXTURE_CUBE_MAP, GLsizei(mNumMipmaps+1), internalformat, GLsizei(width), GLsizei(height)));
                    break;
                case TEX_TYPE_2D_ARRAY:
                case TEX_TYPE_3D:
                    OGRE_CHECK_GL_ERROR(glTexStorage3D(getGLES2TextureTarget(), GLsizei(mNumMipmaps+1), internalformat, GLsizei(width), GLsizei(height), GLsizei(depth)));
                    break;
            }
            return;
        }
        GLenum datatype = GLES2PixelUtil::getGLOriginDataType(mFormat);

        // Run through this process to pregenerate mipmap pyramid
        for(uint32 mip = 0; mip <= mNumMipmaps; mip++)
        {
#if OGRE_DEBUG_MODE
            LogManager::getSingleton().logMessage("GLES2Texture::create - Mip: " + StringConverter::toString(mip) +
                                                  " Name: " + mName +
                                                  " ID: " + StringConverter::toString(mTextureID) +
                                                  " Width: " + StringConverter::toString(width) +
                                                  " Height: " + StringConverter::toString(height) +
                                                  " Internal Format: " + StringConverter::toString(internalformat, 0, ' ', std::ios::hex) +
                                                  " Format: " + StringConverter::toString(format, 0, ' ', std::ios::hex) +
                                                  " Datatype: " + StringConverter::toString(datatype, 0, ' ', std::ios::hex)
                                                  );
#endif
            // Normal formats
            switch(mTextureType)
            {
                case TEX_TYPE_1D:
                case TEX_TYPE_2D:
#if OGRE_PLATFORM == OGRE_PLATFORM_NACL
                    if(internalformat != format)
                    {
                        LogManager::getSingleton().logMessage("glTexImage2D: format != internalFormat, "
                            "format=" + StringConverter::toString(format) +
                            ", internalFormat=" + StringConverter::toString(internalformat));
                    }
#endif
                    OGRE_CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D,
                                 mip,
                                 internalformat,
                                 width, height,
                                 0,
                                 format,
                                 datatype, 0));
                    break;
                case TEX_TYPE_2D_ARRAY:
                    if(!hasGLES30)
                        break;
                    /* no break */
                case TEX_TYPE_3D:
                    OGRE_CHECK_GL_ERROR(glTexImage3DOES(getGLES2TextureTarget(),
                                 mip,
                                 internalformat,
                                 width, height, depth,
                                 0,
                                 format,
                                 datatype, 0));
                    break;
                case TEX_TYPE_CUBE_MAP:
                    for(int face = 0; face < 6; face++) {
                        OGRE_CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, internalformat,
                            width, height, 0,
                            format, datatype, 0));
                    }
                    break;
                default:
                    break;
            };

            if (width > 1)
            {
                width = Bitwise::firstPO2From(width / 2);
            }
            if (height > 1)
            {
                height = Bitwise::firstPO2From(height / 2);
            }
        }
    }
    
    // Creation / loading methods
    void GLES2Texture::createInternalResourcesImpl(void)
    {
        _createGLTexResource();
        
        _createSurfaceList();

        // Get final internal format
        mFormat = getBuffer(0,0)->getFormat();
    }

    void GLES2Texture::loadImpl()
    {
        if (mUsage & TU_RENDERTARGET)
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

        for (size_t i = 0; i < loadedImages.size(); ++i)
        {
            imagePtrs.push_back(&loadedImages[i]);
        }

        _loadImages(imagePtrs);
    }

    void GLES2Texture::freeInternalResourcesImpl()
    {
        mSurfaceList.clear();
        if (GLES2StateCacheManager* stateCacheManager = mRenderSystem->_getStateCacheManager())
        {
            OGRE_CHECK_GL_ERROR(glDeleteTextures(1, &mTextureID));
            stateCacheManager->invalidateStateForTexture(mTextureID);
        }
        mTextureID = 0;
    }
    
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
    void GLES2Texture::notifyOnContextLost()
    {
        if (!mIsManual) 
        {
            freeInternalResources();
        }
        else
        {
            OGRE_CHECK_GL_ERROR(glDeleteTextures(1, &mTextureID));
            mTextureID = 0;
        }
    }
    
    void GLES2Texture::notifyOnContextReset()
    {
        if (!mIsManual) 
        {
            reload();
        }
        else
        {
            preLoadImpl();
            
            _createGLTexResource();
            
            for(size_t i = 0; i < mSurfaceList.size(); i++)
            {
                static_cast<GLES2TextureBuffer*>(mSurfaceList[i].get())->updateTextureId(mTextureID);
            }
            
            if (mLoader)
            {
                mLoader->loadResource(this);
            }
            
            postLoadImpl();
        }
    }
#endif

    void GLES2Texture::_createSurfaceList()
    {
        mSurfaceList.clear();

        uint32 depth = mDepth;

        // For all faces and mipmaps, store surfaces as HardwarePixelBufferSharedPtr
        for (size_t face = 0; face < getNumFaces(); face++)
        {
            uint32 width = mWidth;
            uint32 height = mHeight;

            for (uint32 mip = 0; mip <= getNumMipmaps(); mip++)
            {
                GLES2HardwarePixelBuffer* buf = OGRE_NEW GLES2TextureBuffer(
                    this, static_cast<GLint>(face), mip, width, height, depth);

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
