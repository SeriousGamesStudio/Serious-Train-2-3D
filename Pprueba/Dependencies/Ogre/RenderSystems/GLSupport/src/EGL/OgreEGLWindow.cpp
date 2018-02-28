/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2008 Renato Araujo Oliveira Filho <renatox@gmail.com>
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
--------------------------------------------------------------------------*/

#include "OgreRoot.h"
#include "OgreException.h"
#include "OgreStringConverter.h"
#include "OgreWindowEventUtilities.h"

#include "OgreGLRenderSystemCommon.h"

#include "OgreEGLSupport.h"
#include "OgreEGLWindow.h"
#include "OgreEGLContext.h"

#include <iostream>
#include <algorithm>
#include <climits>

namespace Ogre {
    EGLWindow::EGLWindow(EGLSupport *glsupport)
        : mGLSupport(glsupport),
          mContext(0),
          mWindow(0),
          mNativeDisplay(0),
          mEglDisplay(EGL_NO_DISPLAY),
          mEglConfig(0),
          mEglSurface(0)
    {
        mIsTopLevel = false;
        mIsFullScreen = false;
        mIsExternal = false;
        mIsExternalGLControl = false;
        mClosed = false;
        mActive = true;//todo
        mIsExternalGLControl = false;
        mVisible = false;
        mVSync = false;
        mVSyncInterval = 1;
    }

    EGLWindow::~EGLWindow()
    {
        destroy();

        if (mContext)
        {
            delete mContext;
        }

        mContext = 0;
    }

    void EGLWindow::destroy(void)
    {
        if (mClosed)
        {
            return;
        }

        mClosed = true;
        mActive = false;

        if (!mIsExternal)
        {
            WindowEventUtilities::_removeRenderWindow(this);
        }

        if (mIsFullScreen)
        {
            mGLSupport->switchMode();
            switchFullScreen(false);
        }
    }

    void EGLWindow::setFullscreen(bool fullscreen, uint width, uint height)
    {
        short frequency = 0;

        if (mClosed || !mIsTopLevel)
        {
            return;
        }

        if (fullscreen == mIsFullScreen && width == mWidth && height == mHeight)
        {
            return;
        }
        if (fullscreen)
        {
            mGLSupport->switchMode(width, height, frequency);
        }
        else
        {
            mGLSupport->switchMode();
        }

        if (mIsFullScreen != fullscreen)
        {
            switchFullScreen(fullscreen);
        }

        if (!mIsFullScreen)
        {
            resize(width, height);
            reposition(mLeft, mTop);
        }
    }

    bool EGLWindow::isClosed() const
    {
        return mClosed;
    }

    bool EGLWindow::isVisible() const
    {
        return mVisible;
    }

    void EGLWindow::setVisible(bool visible)
    {
        mVisible = visible;
    }

  

    void EGLWindow::swapBuffers()
    {
        if (mClosed || mIsExternalGLControl)
        {
            return;
        }

        if (eglSwapBuffers(mEglDisplay, mEglSurface) == EGL_FALSE)
        {
            EGL_CHECK_ERROR
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
                        "Fail to SwapBuffers",
                        __FUNCTION__);
        }
    }

    void EGLWindow::getCustomAttribute( const String& name, void* pData )
    {
        if (name == "DISPLAYNAME")
        {
            *static_cast<String*>(pData) = mGLSupport->getDisplayName();
            return;
        }
        else if (name == "DISPLAY")
        {
            *static_cast<EGLDisplay*>(pData) = mEglDisplay;
            return;
        }
        else if (name == "GLCONTEXT")
        {
            *static_cast<EGLContext**>(pData) = mContext;
            return;
        } 
        else if (name == "WINDOW")
        {
            *static_cast<NativeWindowType*>(pData) = mWindow;
            return;
        } 
    }

    void EGLWindow::copyContentsToMemory(const Box& src, const PixelBox &dst, FrameBuffer buffer)
    {
        if(src.right > mWidth || src.bottom > mHeight || src.front != 0 || src.back != 1
        || dst.getWidth() != src.getWidth() || dst.getHeight() != src.getHeight() || dst.getDepth() != 1)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid box.", "EGLWindow::copyContentsToMemory");
        }

        if (buffer == FB_AUTO)
        {
            buffer = mIsFullScreen? FB_FRONT : FB_BACK;
        }

        static_cast<GLRenderSystemCommon*>(Root::getSingleton().getRenderSystem())
                ->_copyContentsToMemory(getViewport(0), src, dst, buffer);
    }


    ::EGLSurface EGLWindow::createSurfaceFromWindow(::EGLDisplay display,
                                                    NativeWindowType win)
    {
        ::EGLSurface surface;

        surface = eglCreateWindowSurface(display, mEglConfig, (EGLNativeWindowType)win, NULL);
        EGL_CHECK_ERROR

        if (surface == EGL_NO_SURFACE)
        {
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
                        "Fail to create EGLSurface based on NativeWindowType",
                        __FUNCTION__);
        }
        return surface;
    }

    bool EGLWindow::requiresTextureFlipping() const
    {
        return false;
    }

    void EGLWindow::setVSyncEnabled(bool vsync) {
        mVSync = vsync;
        // we need to make our context current to set vsync
        // store previous context to restore when finished.
        ::EGLSurface oldRead = eglGetCurrentSurface(EGL_READ);
        EGL_CHECK_ERROR
        ::EGLSurface oldDraw = eglGetCurrentSurface(EGL_DRAW);
        EGL_CHECK_ERROR
        ::EGLContext  oldContext  = eglGetCurrentContext();
        EGL_CHECK_ERROR
        ::EGLDisplay dpy = mGLSupport->getGLDisplay();

        mContext->setCurrent();

        if (! mIsExternalGLControl )
        {
            eglSwapInterval(dpy, vsync ? mVSyncInterval : 0);
            EGL_CHECK_ERROR
        }

        mContext->endCurrent();

        eglMakeCurrent (dpy, oldDraw, oldRead, oldContext);
        EGL_CHECK_ERROR
    }

    void EGLWindow::setVSyncInterval(unsigned int interval) {
        mVSyncInterval = interval;
        if (mVSync)
            setVSyncEnabled(true);
    }

}
