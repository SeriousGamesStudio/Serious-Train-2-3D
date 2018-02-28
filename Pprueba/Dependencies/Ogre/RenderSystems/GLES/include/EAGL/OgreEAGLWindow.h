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

#ifndef __EAGLWindow_H__
#define __EAGLWindow_H__

#include "OgreRenderWindow.h"

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#import "OgreEAGLView.h"
#import "OgreEAGLViewController.h"

// Forward declarations
@class CAEAGLLayer;

// Define the native window type
typedef UIWindow *NativeWindowType;

#endif

namespace Ogre {
    class EAGLSupport;
    class EAGLESContext;

    class _OgrePrivate EAGLWindow : public RenderWindow
    {
        protected:
            bool mClosed;
            bool mVisible;
            /// Is this using an external window handle?
            bool mIsExternal;
            /// Is this using an external view handle?
            bool mUsingExternalView;
            /// Is this using an external view controller handle?
            bool mUsingExternalViewController;

            // iOS 4 content scaling
            bool mIsContentScalingSupported;
            float mContentScalingFactor;
            float mCurrentOSVersion;

            EAGLSupport* mGLSupport;
            EAGLESContext* mContext;
#ifdef __OBJC__
            NativeWindowType mWindow;
            EAGLView *mView;
            EAGLViewController *mViewController;
#endif

            void createNativeWindow(uint widthPt, uint heightPt, const NameValuePairList *miscParams);
            void reposition(int leftPt, int topPt);
            void resize(unsigned int widthPt, unsigned int heightPt);
            void windowMovedOrResized();
            int _getPixelFromPoint(float viewPt) { return mIsContentScalingSupported ? (int)viewPt * mContentScalingFactor : (int)viewPt; }
            virtual void _beginUpdate();

    public:
            EAGLWindow(EAGLSupport* glsupport);
            virtual ~EAGLWindow();

            float getViewPointToPixelScale() { return mIsContentScalingSupported ? mContentScalingFactor : 1.0f; }
            void create(const String& name, unsigned int widthPt, unsigned int heightPt,
                        bool fullScreen, const NameValuePairList *miscParams);

            virtual void setFullscreen(bool fullscreen, uint widthPt, uint heightPt);
            void destroy(void);
            bool isClosed(void) const { return mClosed; }
            bool isVisible(void) const { return mVisible; }

            void setVisible(bool visible) { mVisible = visible; }
            void setClosed(bool closed) { mClosed = closed; }
            void swapBuffers(void);
            void copyContentsToMemory(const Box& src, const PixelBox &dst, FrameBuffer buffer);

            /**
               @remarks
               * Get custom attribute; the following attributes are valid:
               * WINDOW         The NativeWindowType target for rendering.
               * VIEW           The EAGLView object that is drawn into.
               * VIEWCONTROLLER The UIViewController used for handling view rotation.
               * GLCONTEXT      The Ogre GLESContext used for rendering.
               * SHAREGROUP     The EAGLShareGroup object associated with the main context.
               */
            virtual void getCustomAttribute(const String& name, void* pData);

            bool requiresTextureFlipping() const { return false; }
    };
}

#endif
