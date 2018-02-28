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
#include "OgreStableHeaders.h"
#include "OgreSceneNode.h"

#include "OgreException.h"
#include "OgreCamera.h"
#include "OgreMath.h"
#include "OgreSceneManager.h"
#include "OgreMovableObject.h"
#include "OgreWireBoundingBox.h"

#if OGRE_NODE_STORAGE_LEGACY
#define ITER_VAL(it) it->second
#else
#define ITER_VAL(it) (*it)
#endif

namespace Ogre {
    //-----------------------------------------------------------------------
    SceneNode::SceneNode(SceneManager* creator)
        : Node()
        , mWireBoundingBox(0)
        , mShowBoundingBox(false)
        , mHideBoundingBox(false)
        , mCreator(creator)
        , mYawFixed(false)
        , mAutoTrackTarget(0)
        , mIsInSceneGraph(false)
    {
        needUpdate();
    }
    //-----------------------------------------------------------------------
    SceneNode::SceneNode(SceneManager* creator, const String& name)
        : Node(name)
        , mWireBoundingBox(0)
        , mShowBoundingBox(false)
        , mHideBoundingBox(false)
        , mCreator(creator)
        , mYawFixed(false)
        , mAutoTrackTarget(0)
        , mIsInSceneGraph(false)
    {
        needUpdate();
    }
    //-----------------------------------------------------------------------
    SceneNode::~SceneNode()
    {
        // Detach all objects, do this manually to avoid needUpdate() call 
        // which can fail because of deleted items
        ObjectMap::iterator itr;
        for ( itr = mObjectsByName.begin(); itr != mObjectsByName.end(); ++itr )
        {
            MovableObject* ret = ITER_VAL(itr);
            ret->_notifyAttached((SceneNode*)0);
        }
        mObjectsByName.clear();

        OGRE_DELETE mWireBoundingBox;
    }
    //-----------------------------------------------------------------------
    void SceneNode::_update(bool updateChildren, bool parentHasChanged)
    {
        Node::_update(updateChildren, parentHasChanged);
        _updateBounds();
    }
    //-----------------------------------------------------------------------
    void SceneNode::setParent(Node* parent)
    {
        Node::setParent(parent);

        if (parent)
        {
            SceneNode* sceneParent = static_cast<SceneNode*>(parent);
            setInSceneGraph(sceneParent->isInSceneGraph());
        }
        else
        {
            setInSceneGraph(false);
        }
    }
    //-----------------------------------------------------------------------
    void SceneNode::setInSceneGraph(bool inGraph)
    {
        if (inGraph != mIsInSceneGraph)
        {
            mIsInSceneGraph = inGraph;
            // Tell children
            ChildNodeMap::iterator child;
            for (child = mChildren.begin(); child != mChildren.end(); ++child)
            {
                SceneNode* sceneChild = static_cast<SceneNode*>(ITER_VAL(child));
                sceneChild->setInSceneGraph(inGraph);
            }
        }
    }
    //-----------------------------------------------------------------------
    struct MovableObjectNameExists {
        const String& name;
        bool operator()(const MovableObject* mo) {
            return mo->getName() == name;
        }
    };
    void SceneNode::attachObject(MovableObject* obj)
    {
        if (obj->isAttached())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                "Object already attached to a SceneNode or a Bone",
                "SceneNode::attachObject");
        }

        obj->_notifyAttached(this);

        // Also add to name index
#if OGRE_NODE_STORAGE_LEGACY
        std::pair<ObjectMap::iterator, bool> insresult = 
            mObjectsByName.insert(ObjectMap::value_type(obj->getName(), obj));
        OgreAssert(insresult.second, "Object was not attached because an object of the "
            "same name was already attached to this node.");
        (void)insresult;
#else
        MovableObjectNameExists pred = {obj->getName()};
        ObjectMap::iterator it = std::find_if(mObjectsByName.begin(), mObjectsByName.end(), pred);
        OgreAssert(it == mObjectsByName.end(), "Object was not attached because an object of the "
                                               "same name was already attached to this node.");
        mObjectsByName.push_back(obj);
#endif
        // Make sure bounds get updated (must go right to the top)
        needUpdate();
    }
    //-----------------------------------------------------------------------
    unsigned short SceneNode::numAttachedObjects(void) const
    {
        return static_cast< unsigned short >( mObjectsByName.size() );
    }
    //-----------------------------------------------------------------------
    MovableObject* SceneNode::getAttachedObject(unsigned short index)
    {
        if (index < mObjectsByName.size())
        {
#if OGRE_NODE_STORAGE_LEGACY
            ObjectMap::iterator i = mObjectsByName.begin();
            // Increment (must do this one at a time)            
            while (index--)++i;

            return i->second;
#else
            return mObjectsByName[index];
#endif
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Object index out of bounds.", "SceneNode::getAttachedObject");
        }
    }
    //-----------------------------------------------------------------------
    MovableObject* SceneNode::getAttachedObject(const String& name)
    {
        // Look up 
#if OGRE_NODE_STORAGE_LEGACY
        ObjectMap::iterator i = mObjectsByName.find(name);
#else
        MovableObjectNameExists pred = {name};
        ObjectMap::iterator i = std::find_if(mObjectsByName.begin(), mObjectsByName.end(), pred);
#endif

        if (i == mObjectsByName.end())
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Attached object " + 
                name + " not found.", "SceneNode::getAttachedObject");
        }

        return ITER_VAL(i);
    }
    //-----------------------------------------------------------------------
    MovableObject* SceneNode::detachObject(unsigned short index)
    {
        if (index < mObjectsByName.size())
        {

            ObjectMap::iterator i = mObjectsByName.begin();
#if OGRE_NODE_STORAGE_LEGACY
            // Increment (must do this one at a time)
            while (index--)++i;
#else
            i += index;
#endif
            MovableObject* ret = ITER_VAL(i);

#if OGRE_NODE_STORAGE_LEGACY
            mObjectsByName.erase(i);
#else
            std::swap(*i, mObjectsByName.back());
            mObjectsByName.pop_back();
#endif

            ret->_notifyAttached((SceneNode*)0);

            // Make sure bounds get updated (must go right to the top)
            needUpdate();

            return ret;
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Object index out of bounds.", "SceneNode::getAttchedEntity");
        }

    }
    //-----------------------------------------------------------------------
    MovableObject* SceneNode::detachObject(const String& name)
    {
#if OGRE_NODE_STORAGE_LEGACY
        ObjectMap::iterator it = mObjectsByName.find(name);
#else
        MovableObjectNameExists pred = {name};
        ObjectMap::iterator it = std::find_if(mObjectsByName.begin(), mObjectsByName.end(), pred);
#endif
        if (it == mObjectsByName.end())
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Object " + name + " is not attached "
                "to this node.", "SceneNode::detachObject");
        }

        MovableObject* ret = ITER_VAL(it);

#if OGRE_NODE_STORAGE_LEGACY
        mObjectsByName.erase(it);
#else
        std::swap(*it, mObjectsByName.back());
        mObjectsByName.pop_back();
#endif
        ret->_notifyAttached((SceneNode*)0);
        // Make sure bounds get updated (must go right to the top)
        needUpdate();
        
        return ret;

    }
    //-----------------------------------------------------------------------
    void SceneNode::detachObject(MovableObject* obj)
    {
        ObjectMap::iterator i, iend;
        iend = mObjectsByName.end();
        for (i = mObjectsByName.begin(); i != iend; ++i)
        {
            if (ITER_VAL(i) == obj)
            {
#if OGRE_NODE_STORAGE_LEGACY
                mObjectsByName.erase(i);
#else
                std::swap(*i, mObjectsByName.back());
                mObjectsByName.pop_back();
#endif
                break;
            }
        }
        obj->_notifyAttached((SceneNode*)0);

        // Make sure bounds get updated (must go right to the top)
        needUpdate();

    }
    //-----------------------------------------------------------------------
    void SceneNode::detachAllObjects(void)
    {
        ObjectMap::iterator itr;
        for ( itr = mObjectsByName.begin(); itr != mObjectsByName.end(); ++itr )
        {
            MovableObject* ret = ITER_VAL(itr);
            ret->_notifyAttached((SceneNode*)0);
        }
        mObjectsByName.clear();
        // Make sure bounds get updated (must go right to the top)
        needUpdate();
    }
    //-----------------------------------------------------------------------
    void SceneNode::_updateBounds(void)
    {
        // Reset bounds first
        mWorldAABB.setNull();

        // Update bounds from own attached objects
        ObjectMap::iterator i;
        for (i = mObjectsByName.begin(); i != mObjectsByName.end(); ++i)
        {
            // Merge world bounds of each object
            mWorldAABB.merge(ITER_VAL(i)->getWorldBoundingBox(true));
        }

        // Merge with children
        ChildNodeMap::iterator child;
        for (child = mChildren.begin(); child != mChildren.end(); ++child)
        {
            SceneNode* sceneChild = static_cast<SceneNode*>(ITER_VAL(child));
            mWorldAABB.merge(sceneChild->mWorldAABB);
        }

    }
    //-----------------------------------------------------------------------
    void SceneNode::_findVisibleObjects(Camera* cam, RenderQueue* queue, 
        VisibleObjectsBoundsInfo* visibleBounds, bool includeChildren, 
        bool displayNodes, bool onlyShadowCasters)
    {
        // Check self visible
        if (!cam->isVisible(mWorldAABB))
            return;

        // Add all entities
        ObjectMap::iterator iobj;
        ObjectMap::iterator iobjend = mObjectsByName.end();
        for (iobj = mObjectsByName.begin(); iobj != iobjend; ++iobj)
        {
            MovableObject* mo = ITER_VAL(iobj);

            queue->processVisibleObject(mo, cam, onlyShadowCasters, visibleBounds);
        }

        if (includeChildren)
        {
            ChildNodeMap::iterator child, childend;
            childend = mChildren.end();
            for (child = mChildren.begin(); child != childend; ++child)
            {
                SceneNode* sceneChild = static_cast<SceneNode*>(ITER_VAL(child));
                sceneChild->_findVisibleObjects(cam, queue, visibleBounds, includeChildren, 
                    displayNodes, onlyShadowCasters);
            }
        }

        if (displayNodes)
        {
            // Include self in the render queue
            queue->addRenderable(getDebugRenderable());
        }

        // Check if the bounding box should be shown.
        // See if our flag is set or if the scene manager flag is set.
        if ( !mHideBoundingBox &&
             (mShowBoundingBox || (mCreator && mCreator->getShowBoundingBoxes())) )
        { 
            _addBoundingBoxToQueue(queue);
        }


    }

    Node::DebugRenderable* SceneNode::getDebugRenderable()
    {
        Vector3 hs = mWorldAABB.getHalfSize();
        Real sz = std::min(hs.x, hs.y);
        sz = std::min(sz, hs.z);
        sz = std::max(sz, (Real)1.0);
        return Node::getDebugRenderable(sz);
    }


    void SceneNode::_addBoundingBoxToQueue(RenderQueue* queue) {
        // Create a WireBoundingBox if needed.
        if (mWireBoundingBox == NULL) {
            mWireBoundingBox = OGRE_NEW WireBoundingBox();
        }
        mWireBoundingBox->setupBoundingBox(mWorldAABB);
        queue->addRenderable(mWireBoundingBox);
    }

    //-----------------------------------------------------------------------
    void SceneNode::updateFromParentImpl(void) const
    {
        Node::updateFromParentImpl();

        // Notify objects that it has been moved
        ObjectMap::const_iterator i;
        for (i = mObjectsByName.begin(); i != mObjectsByName.end(); ++i)
        {
            MovableObject* object = ITER_VAL(i);
            object->_notifyMoved();
        }
    }
    //-----------------------------------------------------------------------
    Node* SceneNode::createChildImpl(void)
    {
        assert(mCreator);
        return mCreator->createSceneNode();
    }
    //-----------------------------------------------------------------------
    Node* SceneNode::createChildImpl(const String& name)
    {
        assert(mCreator);
        return mCreator->createSceneNode(name);
    }
    //-----------------------------------------------------------------------
    void SceneNode::removeAndDestroyChild(const String& name)
    {
        SceneNode* pChild = static_cast<SceneNode*>(getChild(name));
        pChild->removeAndDestroyAllChildren();

        removeChild(name);
        pChild->getCreator()->destroySceneNode(name);

    }
    //-----------------------------------------------------------------------
    void SceneNode::removeAndDestroyChild(unsigned short index)
    {
#if OGRE_NODE_STORAGE_LEGACY
        SceneNode* pChild = static_cast<SceneNode*>(getChild(index));
#else
        SceneNode* pChild = static_cast<SceneNode*>(mChildren[index]);
#endif
        pChild->removeAndDestroyAllChildren();

        removeChild(index);
        pChild->getCreator()->destroySceneNode(pChild);
    }
    //-----------------------------------------------------------------------
    void SceneNode::removeAndDestroyChild(SceneNode* child)
    {
#if OGRE_NODE_STORAGE_LEGACY
        removeAndDestroyChild(child->getName());
#else
        removeAndDestroyChild(std::find(mChildren.begin(), mChildren.end(), child) - mChildren.begin());
#endif
    }
    //-----------------------------------------------------------------------
    void SceneNode::removeAndDestroyAllChildren(void)
    {
        // do not store iterators (invalidated by
        // SceneManager::destroySceneNode because it causes removal from parent)
        while(!mChildren.empty()) {
            SceneNode* sn = static_cast<SceneNode*>(ITER_VAL(mChildren.begin()));
            sn->removeAndDestroyAllChildren();
            sn->getCreator()->destroySceneNode(sn);
        }

        mChildren.clear();
        needUpdate();
    }
    //-----------------------------------------------------------------------
    SceneNode* SceneNode::createChildSceneNode(const Vector3& inTranslate, 
        const Quaternion& inRotate)
    {
        return static_cast<SceneNode*>(this->createChild(inTranslate, inRotate));
    }
    //-----------------------------------------------------------------------
    SceneNode* SceneNode::createChildSceneNode(const String& name, const Vector3& inTranslate, 
        const Quaternion& inRotate)
    {
        return static_cast<SceneNode*>(this->createChild(name, inTranslate, inRotate));
    }
    //-----------------------------------------------------------------------
    void SceneNode::findLights(LightList& destList, Real radius, uint32 lightMask) const
    {
        // No any optimisation here, hope inherits more smart for that.
        //
        // If a scene node is static and lights have moved, light list won't change
        // can't use a simple global boolean flag since this is only called for
        // visible nodes, so temporarily visible nodes will not be updated
        // Since this is only called for visible nodes, skip the check for now
        //
        if (mCreator)
        {
            // Use SceneManager to calculate
            mCreator->_populateLightList(this, radius, destList, lightMask);
        }
        else
        {
            destList.clear();
        }
    }
    //-----------------------------------------------------------------------
    void SceneNode::setAutoTracking(bool enabled, SceneNode* const target, 
        const Vector3& localDirectionVector,
        const Vector3& offset)
    {
        if (enabled)
        {
            mAutoTrackTarget = target;
            mAutoTrackOffset = offset;
            mAutoTrackLocalDirection = localDirectionVector;
        }
        else
        {
            mAutoTrackTarget = 0;
        }
        if (mCreator)
            mCreator->_notifyAutotrackingSceneNode(this, enabled);
    }
    //-----------------------------------------------------------------------
    void SceneNode::setFixedYawAxis(bool useFixed, const Vector3& fixedAxis)
    {
        mYawFixed = useFixed;
        mYawFixedAxis = fixedAxis;
    }

    //-----------------------------------------------------------------------
    void SceneNode::yaw(const Radian& angle, TransformSpace relativeTo)
    {
        if (mYawFixed)
        {
            rotate(mYawFixedAxis, angle, relativeTo);
        }
        else
        {
            rotate(Vector3::UNIT_Y, angle, relativeTo);
        }

    }
    //-----------------------------------------------------------------------
    void SceneNode::setDirection(Real x, Real y, Real z, TransformSpace relativeTo, 
        const Vector3& localDirectionVector)
    {
        setDirection(Vector3(x,y,z), relativeTo, localDirectionVector);
    }

    //-----------------------------------------------------------------------
    void SceneNode::setDirection(const Vector3& vec, TransformSpace relativeTo, 
        const Vector3& localDirectionVector)
    {
        // Do nothing if given a zero vector
        if (vec == Vector3::ZERO) return;

        // The direction we want the local direction point to
        Vector3 targetDir = vec.normalisedCopy();

        // Transform target direction to world space
        switch (relativeTo)
        {
        case TS_PARENT:
            if (mInheritOrientation)
            {
                if (mParent)
                {
                    targetDir = mParent->_getDerivedOrientation() * targetDir;
                }
            }
            break;
        case TS_LOCAL:
            targetDir = _getDerivedOrientation() * targetDir;
            break;
        case TS_WORLD:
            // default orientation
            break;
        }

        // Calculate target orientation relative to world space
        Quaternion targetOrientation;
        if( mYawFixed )
        {
            // Calculate the quaternion for rotate local Z to target direction
            Vector3 yawAxis = mYawFixedAxis;

            if (mInheritOrientation && mParent)
            {
                yawAxis = mParent->_getDerivedOrientation() * yawAxis;
            }

            Vector3 xVec = yawAxis.crossProduct(targetDir);
            xVec.normalise();
            Vector3 yVec = targetDir.crossProduct(xVec);
            yVec.normalise();
            Quaternion unitZToTarget = Quaternion(xVec, yVec, targetDir);

            if (localDirectionVector == Vector3::NEGATIVE_UNIT_Z)
            {
                // Specail case for avoid calculate 180 degree turn
                targetOrientation =
                    Quaternion(-unitZToTarget.y, -unitZToTarget.z, unitZToTarget.w, unitZToTarget.x);
            }
            else
            {
                // Calculate the quaternion for rotate local direction to target direction
                Quaternion localToUnitZ = localDirectionVector.getRotationTo(Vector3::UNIT_Z);
                targetOrientation = unitZToTarget * localToUnitZ;
            }
        }
        else
        {
            const Quaternion& currentOrient = _getDerivedOrientation();

            // Get current local direction relative to world space
            Vector3 currentDir = currentOrient * localDirectionVector;

            if ((currentDir+targetDir).squaredLength() < 0.00005f)
            {
                // Oops, a 180 degree turn (infinite possible rotation axes)
                // Default to yaw i.e. use current UP
                targetOrientation =
                    Quaternion(-currentOrient.y, -currentOrient.z, currentOrient.w, currentOrient.x);
            }
            else
            {
                // Derive shortest arc to new direction
                Quaternion rotQuat = currentDir.getRotationTo(targetDir);
                targetOrientation = rotQuat * currentOrient;
            }
        }

        // Set target orientation, transformed to parent space
        if (mParent && mInheritOrientation)
            setOrientation(mParent->_getDerivedOrientation().UnitInverse() * targetOrientation);
        else
            setOrientation(targetOrientation);
    }
    //-----------------------------------------------------------------------
    void SceneNode::lookAt( const Vector3& targetPoint, TransformSpace relativeTo, 
        const Vector3& localDirectionVector)
    {
        // Calculate ourself origin relative to the given transform space
        Vector3 origin;
        switch (relativeTo)
        {
        default:    // Just in case
        case TS_WORLD:
            origin = _getDerivedPosition();
            break;
        case TS_PARENT:
            origin = mPosition;
            break;
        case TS_LOCAL:
            origin = Vector3::ZERO;
            break;
        }

        setDirection(targetPoint - origin, relativeTo, localDirectionVector);
    }
    //-----------------------------------------------------------------------
    void SceneNode::_autoTrack(void)
    {
        // NB assumes that all scene nodes have been updated
        if (mAutoTrackTarget)
        {
            lookAt(mAutoTrackTarget->_getDerivedPosition() + mAutoTrackOffset, 
                TS_WORLD, mAutoTrackLocalDirection);
            // update self & children
            _update(true, true);
        }
    }
    //-----------------------------------------------------------------------
    SceneNode* SceneNode::getParentSceneNode(void) const
    {
        return static_cast<SceneNode*>(getParent());
    }
    //-----------------------------------------------------------------------
    void SceneNode::setVisible(bool visible, bool cascade)
    {
        ObjectMap::iterator oi, oiend;
        oiend = mObjectsByName.end();
        for (oi = mObjectsByName.begin(); oi != oiend; ++oi)
        {
            ITER_VAL(oi)->setVisible(visible);
        }

        if (cascade)
        {
            ChildNodeMap::iterator i, iend;
            iend = mChildren.end();
            for (i = mChildren.begin(); i != iend; ++i)
            {
                static_cast<SceneNode*>(ITER_VAL(i))->setVisible(visible, cascade);
            }
        }
    }
    //-----------------------------------------------------------------------
    void SceneNode::setDebugDisplayEnabled(bool enabled, bool cascade)
    {
        ObjectMap::iterator oi, oiend;
        oiend = mObjectsByName.end();
        for (oi = mObjectsByName.begin(); oi != oiend; ++oi)
        {
            ITER_VAL(oi)->setDebugDisplayEnabled(enabled);
        }

        if (cascade)
        {
            ChildNodeMap::iterator i, iend;
            iend = mChildren.end();
            for (i = mChildren.begin(); i != iend; ++i)
            {
                static_cast<SceneNode*>(ITER_VAL(i))->setDebugDisplayEnabled(enabled, cascade);
            }
        }
    }
    //-----------------------------------------------------------------------
    void SceneNode::flipVisibility(bool cascade)
    {
        ObjectMap::iterator oi, oiend;
        oiend = mObjectsByName.end();
        for (oi = mObjectsByName.begin(); oi != oiend; ++oi)
        {
            ITER_VAL(oi)->setVisible(!ITER_VAL(oi)->getVisible());
        }

        if (cascade)
        {
            ChildNodeMap::iterator i, iend;
            iend = mChildren.end();
            for (i = mChildren.begin(); i != iend; ++i)
            {
                static_cast<SceneNode*>(ITER_VAL(i))->flipVisibility(cascade);
            }
        }
    }



}
