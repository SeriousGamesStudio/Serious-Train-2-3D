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

#include "OgreGLES2HardwareBufferManager.h"
#include "OgreGLES2HardwareVertexBuffer.h"
#include "OgreGLES2HardwareIndexBuffer.h"
#include "OgreGLES2HardwareUniformBuffer.h"
#include "OgreGLES2RenderToVertexBuffer.h"
#include "OgreGLES2RenderSystem.h"
#include "OgreGLVertexArrayObject.h"
#include "OgreGLUtil.h"
#include "OgreRoot.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    GLES2HardwareBufferManagerBase::GLES2HardwareBufferManagerBase()
    {
        mRenderSystem = getGLES2RenderSystem();
    }

    GLES2HardwareBufferManagerBase::~GLES2HardwareBufferManagerBase()
    {
        destroyAllDeclarations();
        destroyAllBindings();
    }

    GLES2StateCacheManager * GLES2HardwareBufferManagerBase::getStateCacheManager()
    {
        return mRenderSystem->_getStateCacheManager();
    }

    void GLES2HardwareBufferManagerBase::notifyContextDestroyed(GLContext* context)
    {
        OGRE_LOCK_MUTEX(mVertexDeclarationsMutex);
        for(VertexDeclarationList::iterator it = mVertexDeclarations.begin(), it_end = mVertexDeclarations.end(); it != it_end; ++it)
            static_cast<GLVertexArrayObject*>(*it)->notifyContextDestroyed(context);
    }

    HardwareVertexBufferSharedPtr
        GLES2HardwareBufferManagerBase::createVertexBuffer(size_t vertexSize,
                                                      size_t numVerts,
                                                      HardwareBuffer::Usage usage,
                                                      bool useShadowBuffer)
    {
        if(!mRenderSystem->getCapabilities()->hasCapability(RSC_MAPBUFFER))
            useShadowBuffer = true;

        GLES2HardwareVertexBuffer* buf = OGRE_NEW GLES2HardwareVertexBuffer(this, vertexSize, numVerts, usage, useShadowBuffer);

        {
            OGRE_LOCK_MUTEX(mVertexBuffersMutex);
            mVertexBuffers.insert(buf);
        }
        return HardwareVertexBufferSharedPtr(buf);
    }

    HardwareIndexBufferSharedPtr GLES2HardwareBufferManagerBase::createIndexBuffer(HardwareIndexBuffer::IndexType itype,
                                                                              size_t numIndexes,
                                                                              HardwareBuffer::Usage usage,
                                                                              bool useShadowBuffer)
    {
        if(!mRenderSystem->getCapabilities()->hasCapability(RSC_MAPBUFFER))
            useShadowBuffer = true;

        GLES2HardwareIndexBuffer* buf = OGRE_NEW GLES2HardwareIndexBuffer(this, itype, numIndexes, usage, useShadowBuffer);

        {
            OGRE_LOCK_MUTEX(mIndexBuffersMutex);
            mIndexBuffers.insert(buf);
        }
        return HardwareIndexBufferSharedPtr(buf);
    }

    RenderToVertexBufferSharedPtr GLES2HardwareBufferManagerBase::createRenderToVertexBuffer()
    {
        if(mRenderSystem->hasMinGLVersion(3, 0))
            return RenderToVertexBufferSharedPtr(new GLES2RenderToVertexBuffer());

        // not supported
        return RenderToVertexBufferSharedPtr();
    }

    VertexDeclaration* GLES2HardwareBufferManagerBase::createVertexDeclarationImpl(void)
    {
        return OGRE_NEW GLVertexArrayObject();
    }

    void GLES2HardwareBufferManagerBase::destroyVertexDeclarationImpl(VertexDeclaration* decl)
    {
        if(decl)
            OGRE_DELETE decl;
    }

    GLenum GLES2HardwareBufferManagerBase::getGLType(VertexElementType type)
    {
        switch(type)
        {
            case VET_FLOAT1:
            case VET_FLOAT2:
            case VET_FLOAT3:
            case VET_FLOAT4:
                return GL_FLOAT;
            case VET_SHORT1:
            case VET_SHORT2:
            case VET_SHORT3:
            case VET_SHORT4:
            case VET_SHORT2_NORM:
            case VET_SHORT4_NORM:
                return GL_SHORT;
            case VET_COLOUR:
            case VET_COLOUR_ABGR:
            case VET_COLOUR_ARGB:
            case VET_UBYTE4:
            case VET_UBYTE4_NORM:
                return GL_UNSIGNED_BYTE;
            case VET_BYTE4:
            case VET_BYTE4_NORM:
                return GL_BYTE;
            case VET_INT1:
            case VET_INT2:
            case VET_INT3:
            case VET_INT4:
                return GL_INT;
            case VET_UINT1:
            case VET_UINT2:
            case VET_UINT3:
            case VET_UINT4:
                return GL_UNSIGNED_INT;
            case VET_USHORT1:
            case VET_USHORT2:
            case VET_USHORT3:
            case VET_USHORT4:
            case VET_USHORT2_NORM:
            case VET_USHORT4_NORM:
                return GL_UNSIGNED_SHORT;
            case VET_DOUBLE1:
            case VET_DOUBLE2:
            case VET_DOUBLE3:
            case VET_DOUBLE4:
                return 0;
        };

        OgreAssert(false, "unknown Vertex Element Type");
        return 0;
    }

    //---------------------------------------------------------------------
    Ogre::HardwareUniformBufferSharedPtr GLES2HardwareBufferManagerBase::createUniformBuffer( size_t sizeBytes, HardwareBuffer::Usage usage, bool useShadowBuffer, const String& name )
    {
        if(!mRenderSystem->hasMinGLVersion(3, 0))
        {
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
                "GLES2 does not support uniform buffer objects",
                "GLES2HardwareBufferManagerBase::createUniformBuffer");
        }

        GLES2HardwareUniformBuffer* buf =
            new GLES2HardwareUniformBuffer(this, sizeBytes, usage, useShadowBuffer, name);
        {
            OGRE_LOCK_MUTEX(mUniformBuffersMutex);
            mUniformBuffers.insert(buf);
        }
        return HardwareUniformBufferSharedPtr(buf);
    }
    //---------------------------------------------------------------------
    Ogre::HardwareCounterBufferSharedPtr GLES2HardwareBufferManagerBase::createCounterBuffer( size_t sizeBytes, HardwareBuffer::Usage usage, bool useShadowBuffer, const String& name )
    {
        OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
                    "GLES2 does not support atomic counter buffers",
                    "GLES2HardwareBufferManagerBase::createCounterBuffer");
    }

}
