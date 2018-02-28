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
#ifndef __SampleContext_H__
#define __SampleContext_H__

#include "OgreApplicationContext.h"

#include "Sample.h"

namespace OgreBites
{
    /*=============================================================================
    | Base class responsible for setting up a common context for samples.
    | May be subclassed for specific sample types (not specific samples).
    | Allows one sample to run at a time, while maintaining a sample queue.
    =============================================================================*/
    class SampleContext : public ApplicationContext, public InputListener
    {
    public:
        Ogre::RenderWindow* mWindow;

        SampleContext(const Ogre::String& appName = OGRE_VERSION_NAME)
        : ApplicationContext(appName), mWindow(NULL)
        {
            mCurrentSample = 0;
            mSamplePaused = false;
            mLastRun = false;
            mLastSample = 0;
        }

        virtual Sample* getCurrentSample()
        {
            return mCurrentSample;
        }

        /*-----------------------------------------------------------------------------
        | Quits the current sample and starts a new one.
        -----------------------------------------------------------------------------*/
        virtual void runSample(Sample* s)
        {
#if OGRE_PROFILING
            Ogre::Profiler* prof = Ogre::Profiler::getSingletonPtr();
            if (prof)
                prof->setEnabled(false);
#endif

            if (mCurrentSample)
            {
                mCurrentSample->_shutdown();    // quit current sample
                mSamplePaused = false;          // don't pause the next sample
            }

            mWindow->removeAllViewports();                  // wipe viewports
            mWindow->resetStatistics();

            if (s)
            {
                // retrieve sample's required plugins and currently installed plugins
                Ogre::Root::PluginInstanceList ip = mRoot->getInstalledPlugins();
                Ogre::StringVector rp = s->getRequiredPlugins();

                for (Ogre::StringVector::iterator j = rp.begin(); j != rp.end(); j++)
                {
                    bool found = false;
                    // try to find the required plugin in the current installed plugins
                    for (Ogre::Root::PluginInstanceList::iterator k = ip.begin(); k != ip.end(); k++)
                    {
                        if ((*k)->getName() == *j)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)  // throw an exception if a plugin is not found
                    {
                        Ogre::String desc = "Sample requires plugin: " + *j;
                        Ogre::String src = "SampleContext::runSample";
                        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, desc, src);
                    }
                }

                // throw an exception if samples requires the use of another renderer
                Ogre::String rrs = s->getRequiredRenderSystem();
                if (!rrs.empty() && rrs != mRoot->getRenderSystem()->getName())
                {
                    Ogre::String desc = "Sample only runs with renderer: " + rrs;
                    Ogre::String src = "SampleContext::runSample";
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, desc, src);
                }

                // test system capabilities against sample requirements
                s->testCapabilities(mRoot->getRenderSystem()->getCapabilities());

                s->_setup(mWindow, mFSLayer, mOverlaySystem);   // start new sample
            }
#if OGRE_PROFILING
            if (prof)
                prof->setEnabled(true);
#endif

            mCurrentSample = s;
        }

        /*-----------------------------------------------------------------------------
        | This function encapsulates the entire lifetime of the context.
        -----------------------------------------------------------------------------*/
#if OGRE_PLATFORM != OGRE_PLATFORM_NACL
        virtual void go(Sample* initialSample = 0)
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS || ((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
            createRoot();

            if (!oneTimeConfig()) return;

            if (!mFirstRun) mRoot->setRenderSystem(mRoot->getRenderSystemByName(mNextRenderer));

            mLastRun = true;  // assume this is our last run

            setup();

            if (!mFirstRun) recoverLastSample();
            else if (initialSample) runSample(initialSample);

            mRoot->saveConfig();
#else
            while (!mLastRun)
            {
                mLastRun = true;  // assume this is our last run

                initApp();

#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
                // restore the last sample if there was one or, if not, start initial sample
                if (!mFirstRun) recoverLastSample();
                else if (initialSample) runSample(initialSample);
#endif

                loadStartUpSample();
        
                if (mRoot->getRenderSystem() != NULL)
                {
                    mRoot->startRendering();    // start the render loop
                }

                closeApp();

                mFirstRun = false;
            }
#endif
        }
#endif

        virtual void loadStartUpSample() {}
        
        virtual bool isCurrentSamplePaused()
        {
            if (mCurrentSample) return mSamplePaused;
            return false;
        }

        virtual void pauseCurrentSample()
        {
            if (mCurrentSample && !mSamplePaused)
            {
                mSamplePaused = true;
                mCurrentSample->paused();
            }
        }

        virtual void unpauseCurrentSample()
        {
            if (mCurrentSample && mSamplePaused)
            {
                mSamplePaused = false;
                mCurrentSample->unpaused();
            }
        }
            
        /*-----------------------------------------------------------------------------
        | Processes frame started events.
        -----------------------------------------------------------------------------*/
        virtual bool frameStarted(const Ogre::FrameEvent& evt)
        {
            pollEvents();

            // manually call sample callback to ensure correct order
            return (mCurrentSample && !mSamplePaused) ? mCurrentSample->frameStarted(evt) : true;
        }
            
        /*-----------------------------------------------------------------------------
        | Processes rendering queued events.
        -----------------------------------------------------------------------------*/
        virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt)
        {
            // manually call sample callback to ensure correct order
            return (mCurrentSample && !mSamplePaused) ? mCurrentSample->frameRenderingQueued(evt) : true;
        }
            
        /*-----------------------------------------------------------------------------
        | Processes frame ended events.
        -----------------------------------------------------------------------------*/
        virtual bool frameEnded(const Ogre::FrameEvent& evt)
        {
            // manually call sample callback to ensure correct order
            if (mCurrentSample && !mSamplePaused && !mCurrentSample->frameEnded(evt)) return false;
            // quit if window was closed
            if (mWindow->isClosed()) return false;
            // go into idle mode if current sample has ended
            if (mCurrentSample && mCurrentSample->isDone()) runSample(0);

            return true;
        }

        /*-----------------------------------------------------------------------------
        | Processes window size change event. Adjusts mouse's region to match that
        | of the window. You could also override this method to prevent resizing.
        -----------------------------------------------------------------------------*/
        virtual void windowResized(Ogre::RenderWindow* rw)
        {
            // manually call sample callback to ensure correct order
            if (mCurrentSample && !mSamplePaused) mCurrentSample->windowResized(rw);
        }

        // window event callbacks which manually call their respective sample callbacks to ensure correct order

        virtual void windowMoved(Ogre::RenderWindow* rw)
        {
            if (mCurrentSample && !mSamplePaused) mCurrentSample->windowMoved(rw);
        }

        virtual bool windowClosing(Ogre::RenderWindow* rw)
        {
            if (mCurrentSample && !mSamplePaused) return mCurrentSample->windowClosing(rw);
            return true;
        }

        virtual void windowClosed(Ogre::RenderWindow* rw)
        {
            if (mCurrentSample && !mSamplePaused) mCurrentSample->windowClosed(rw);
        }

        virtual void windowFocusChange(Ogre::RenderWindow* rw)
        {
            if (mCurrentSample && !mSamplePaused) mCurrentSample->windowFocusChange(rw);
        }

        // keyboard and mouse callbacks which manually call their respective sample callbacks to ensure correct order

        virtual bool keyPressed(const KeyboardEvent& evt)
        {
            if (mCurrentSample && !mSamplePaused) return mCurrentSample->keyPressed(evt);
            return true;
        }

        virtual bool keyReleased(const KeyboardEvent& evt)
        {
            if (mCurrentSample && !mSamplePaused) return mCurrentSample->keyReleased(evt);
            return true;
        }

        void transformInputState(TouchFingerEvent &state)
        {
#if 0
            int w = mWindow->getViewport(0)->getActualWidth();
            int h = mWindow->getViewport(0)->getActualHeight();
            int absX = state.X.abs;
            int absY = state.Y.abs;
            int relX = state.X.rel;
            int relY = state.Y.rel;

            // as OIS work in windowing system units we need to convert them to pixels
            float scale = mWindow->getViewPointToPixelScale();
            if(scale != 1.0f)
            {
                absX = (int)(absX * scale);
                absY = (int)(absY * scale);
                relX = (int)(relX * scale);
                relY = (int)(relY * scale);
            }

            // determine required orientation
            Ogre::OrientationMode orientation = Ogre::OR_DEGREE_0;
#    if (OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0)
            orientation = mWindow->getViewport(0)->getOrientationMode();
#    elif (OGRE_NO_VIEWPORT_ORIENTATIONMODE == 1) && (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS)
            UIInterfaceOrientation interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
            switch (interfaceOrientation)
            {
            case UIInterfaceOrientationPortrait:           break;
            case UIInterfaceOrientationLandscapeLeft:      orientation = Ogre::OR_DEGREE_90;  break;
            case UIInterfaceOrientationPortraitUpsideDown: orientation = Ogre::OR_DEGREE_180; break;
            case UIInterfaceOrientationLandscapeRight:     orientation = Ogre::OR_DEGREE_270; break;
            }
#    endif

            // apply changes
            switch (orientation)
            {
            case Ogre::OR_DEGREE_0:
                state.X.abs = absX;
                state.Y.abs = absY;
                state.X.rel = relX;
                state.Y.rel = relY;
                state.width = w;
                state.height = h;
                break;
            case Ogre::OR_DEGREE_90:
                state.X.abs = w - absY;
                state.Y.abs = absX;
                state.X.rel = -relY;
                state.Y.rel = relX;
                state.width = h;
                state.height = w;
                break;
            case Ogre::OR_DEGREE_180:
                state.X.abs = w - absX;
                state.Y.abs = h - absY;
                state.X.rel = -relX;
                state.Y.rel = -relY;
                state.width = w;
                state.height = h;
                break;
            case Ogre::OR_DEGREE_270:
                state.X.abs = absY;
                state.Y.abs = h - absX;
                state.X.rel = relY;
                state.Y.rel = -relX;
                state.width = h;
                state.height = w;
                break;
            }
#endif
        }

        virtual bool touchMoved(const TouchFingerEvent& evt)
        {
            if (mCurrentSample && !mSamplePaused)
                return mCurrentSample->touchMoved(evt);
            return true;
        }

        virtual bool mouseMoved(const MouseMotionEvent& evt)
        {
            // Miniscule mouse movements are still considered hovering.
            // if (evt.xrel > 100000 || evt.yrel > 100000)
            // {
            //     mTimeSinceMouseMoved = 0;
            // }

            if (mCurrentSample && !mSamplePaused)
                return mCurrentSample->mouseMoved(evt);
            return true;
        }

        virtual bool touchPressed(const TouchFingerEvent& evt)
        {
            if (mCurrentSample && !mSamplePaused)
                return mCurrentSample->touchPressed(evt);
            return true;
        }

        virtual bool mousePressed(const MouseButtonEvent& evt)
        {
            if (mCurrentSample && !mSamplePaused)
                return mCurrentSample->mousePressed(evt);
            return true;
        }

        virtual bool touchReleased(const TouchFingerEvent& evt)
        {
            if (mCurrentSample && !mSamplePaused)
                return mCurrentSample->touchReleased(evt);
            return true;
        }

        virtual bool mouseReleased(const MouseButtonEvent& evt)
        {
            if (mCurrentSample && !mSamplePaused)
                return mCurrentSample->mouseReleased(evt);
            return true;
        }

#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS) || (OGRE_PLATFORM == OGRE_PLATFORM_ANDROID)
        //FIXME: Handle mouse wheel wheel events on mobile devices.
        // virtual bool touchReleased(const SDL_TouchFingerEvent& evt)
        // {
        //     if (mCurrentSample && !mSamplePaused)
        //         return mCurrentSample->touchReleased(evt);
        //     return true;
        // }
#endif
        virtual bool mouseWheelRolled(const MouseWheelEvent& evt)
        {
            if (mCurrentSample && !mSamplePaused)
                return mCurrentSample->mouseWheelRolled(evt);
            return true;
        }

        bool isFirstRun() { return mFirstRun; }
        void setFirstRun(bool flag) { mFirstRun = flag; }
        bool isLastRun() { return mLastRun; }
        void setLastRun(bool flag) { mLastRun = flag; }
    protected:
        /*-----------------------------------------------------------------------------
        | Reconfigures the context. Attempts to preserve the current sample state.
        -----------------------------------------------------------------------------*/
        virtual void reconfigure(const Ogre::String& renderer, Ogre::NameValuePairList& options)
        {
            // save current sample state
            mLastSample = mCurrentSample;
            if (mCurrentSample) mCurrentSample->saveState(mLastSampleState);

            mLastRun = false;             // we want to go again with the new settings
            ApplicationContext::reconfigure(renderer, options);
        }

        /*-----------------------------------------------------------------------------
        | Recovers the last sample after a reset. You can override in the case that
        | the last sample is destroyed in the process of resetting, and you have to
        | recover it through another means.
        -----------------------------------------------------------------------------*/
        virtual void recoverLastSample()
        {
            runSample(mLastSample);
            mLastSample->restoreState(mLastSampleState);
            mLastSample = 0;
            mLastSampleState.clear();
        }

        /*-----------------------------------------------------------------------------
        | Cleans up and shuts down the context.
        -----------------------------------------------------------------------------*/
        virtual void shutdown()
        {
            if (mCurrentSample)
            {
                mCurrentSample->_shutdown();
                mCurrentSample = 0;
            }

            ApplicationContext::shutdown();
        }
        
        Sample* mCurrentSample;         // currently running sample
        bool mSamplePaused;             // whether current sample is paused
        bool mLastRun;                  // whether or not this is the final run
        Sample* mLastSample;            // last sample run before reconfiguration
        Ogre::NameValuePairList mLastSampleState;     // state of last sample
    };
}

#endif
