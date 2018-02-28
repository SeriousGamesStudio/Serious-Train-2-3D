/*
 * OgreBitesConfigDialog.cpp
 *
 *  Created on: 06.12.2016
 *      Author: pavel
 */

#include "OgreBitesConfigDialog.h"

namespace OgreBites {
    Ogre::ConfigDialog* getNativeConfigDialog() {
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        static Ogre::ConfigDialog dialog;
        return &dialog;
#else
        return NULL;
#endif
    }
} /* namespace OgreBites */
