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

#include "OgreGL3PlusHardwareBufferManager.h"
#include "OgreGL3PlusHardwareShaderStorageBuffer.h"
#include "OgreRoot.h"
#include "OgreGL3PlusRenderSystem.h"

namespace Ogre {
    GL3PlusHardwareShaderStorageBuffer::GL3PlusHardwareShaderStorageBuffer(
        HardwareBufferManagerBase* mgr,
        size_t bufferSize,
        HardwareBuffer::Usage usage,
        bool useShadowBuffer, const String& name)
        : HardwareUniformBuffer(mgr, bufferSize, usage, useShadowBuffer, name),
          mBuffer(GL_SHADER_STORAGE_BUFFER, mSizeInBytes, usage),
          mBinding(0)
    {
    }

    void GL3PlusHardwareShaderStorageBuffer::setGLBufferBinding(GLint binding)
    {
        mBinding = binding;

        // Attach the buffer to the UBO binding
        OGRE_CHECK_GL_ERROR(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mBinding, getGLBufferId()));
    }

    void GL3PlusHardwareShaderStorageBuffer::readData(size_t offset, size_t length, void* pDest)
    {
        mBuffer.readData(offset, length, pDest);
    }

    void GL3PlusHardwareShaderStorageBuffer::writeData(size_t offset,
                                                       size_t length,
                                                       const void* pSource,
                                                       bool discardWholeBuffer)
    {
        mBuffer.writeData(offset, length, pSource, discardWholeBuffer);
    }

    void GL3PlusHardwareShaderStorageBuffer::copyData(HardwareBuffer& srcBuffer, size_t srcOffset,
                                                      size_t dstOffset, size_t length, bool discardWholeBuffer)
    {
        if (srcBuffer.isSystemMemory())
        {
            HardwareBuffer::copyData(srcBuffer, srcOffset, dstOffset, length, discardWholeBuffer);
        }
        else
        {
            mBuffer.copyData(static_cast<GL3PlusHardwareShaderStorageBuffer&>(srcBuffer).getGLBufferId(),
                                                     srcOffset, dstOffset, length, discardWholeBuffer);
        }
    }
}
