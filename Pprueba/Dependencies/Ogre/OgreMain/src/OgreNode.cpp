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
#include "OgreNode.h"

#include "OgreException.h"
#include "OgreMath.h"

// Dependencies on render-related types due to ability to render node
#include "OgreMaterialManager.h"
#include "OgreMeshManager.h"
#include "OgreSubMesh.h"
#include "OgreCamera.h"
#include "OgreTechnique.h"
#include "OgreManualObject.h"
#include "OgreNameGenerator.h"
#include "OgreMesh.h"

namespace Ogre {

    NameGenerator Node::msNameGenerator("Unnamed_");
    Node::QueuedUpdates Node::msQueuedUpdates;
    //-----------------------------------------------------------------------
    Node::Node()
        :mParent(0),
        mNeedParentUpdate(false),
        mNeedChildUpdate(false),
        mParentNotified(false),
        mQueuedForUpdate(false),
        mOrientation(Quaternion::IDENTITY),
        mPosition(Vector3::ZERO),
        mScale(Vector3::UNIT_SCALE),
        mInheritOrientation(true),
        mInheritScale(true),
        mDerivedOrientation(Quaternion::IDENTITY),
        mDerivedPosition(Vector3::ZERO),
        mDerivedScale(Vector3::UNIT_SCALE),
        mInitialPosition(Vector3::ZERO),
        mInitialOrientation(Quaternion::IDENTITY),
        mInitialScale(Vector3::UNIT_SCALE),
        mCachedTransformOutOfDate(true),
        mListener(0), 
        mDebug(0)
    {
#if OGRE_NODE_STORAGE_LEGACY
        // Generate a name
        mName = msNameGenerator.generate();
#endif

        needUpdate();

    }
    //-----------------------------------------------------------------------
    Node::Node(const String& name)
        :
        mParent(0),
        mNeedParentUpdate(false),
        mNeedChildUpdate(false),
        mParentNotified(false),
        mQueuedForUpdate(false),
        mName(name),
        mOrientation(Quaternion::IDENTITY),
        mPosition(Vector3::ZERO),
        mScale(Vector3::UNIT_SCALE),
        mInheritOrientation(true),
        mInheritScale(true),
        mDerivedOrientation(Quaternion::IDENTITY),
        mDerivedPosition(Vector3::ZERO),
        mDerivedScale(Vector3::UNIT_SCALE),
        mInitialPosition(Vector3::ZERO),
        mInitialOrientation(Quaternion::IDENTITY),
        mInitialScale(Vector3::UNIT_SCALE),
        mCachedTransformOutOfDate(true),
        mListener(0), 
        mDebug(0)

    {

        needUpdate();

    }

    //-----------------------------------------------------------------------
    Node::~Node()
    {
        OGRE_DELETE mDebug;
        mDebug = 0;

        // Call listener (note, only called if there's something to do)
        if (mListener)
        {
            mListener->nodeDestroyed(this);
        }

        removeAllChildren();
        if(mParent)
            mParent->removeChild(this);

        if (mQueuedForUpdate)
        {
            // Erase from queued updates
            QueuedUpdates::iterator it =
                std::find(msQueuedUpdates.begin(), msQueuedUpdates.end(), this);
            assert(it != msQueuedUpdates.end());
            if (it != msQueuedUpdates.end())
            {
                // Optimised algorithm to erase an element from unordered vector.
                *it = msQueuedUpdates.back();
                msQueuedUpdates.pop_back();
            }
        }

    }

    //-----------------------------------------------------------------------
    void Node::setParent(Node* parent)
    {
        bool different = (parent != mParent);

        mParent = parent;
        // Request update from parent
        mParentNotified = false ;
        needUpdate();

        // Call listener (note, only called if there's something to do)
        if (mListener && different)
        {
            if (mParent)
                mListener->nodeAttached(this);
            else
                mListener->nodeDetached(this);
        }

    }

    //-----------------------------------------------------------------------
    const Matrix4& Node::_getFullTransform(void) const
    {
        if (mCachedTransformOutOfDate)
        {
#if OGRE_NODE_INHERIT_TRANSFORM
            Ogre::Matrix4 tr;
            tr.makeTransform(mPosition, mScale, mOrientation);

            if(mParent == NULL)
            {
                mCachedTransform = tr;
            }
            else if(mInheritOrientation && mInheritScale) // everything is inherited
            {
                mCachedTransform = mParent->_getFullTransform() * tr;
            }
            else if(!mInheritOrientation && !mInheritScale) // only position is inherited
            {
                mCachedTransform = tr;
                mCachedTransform.setTrans(tr.getTrans() + mParent->_getFullTransform().getTrans());
            }
            else // shear is inherited together with orientation, controlled by mInheritOrientation
            {
                const Ogre::Matrix4& parentTr = mParent->_getFullTransform();
                Ogre::Vector3 parentScale(
                    parentTr.transformDirectionAffine(Vector3::UNIT_X).length(),
                    parentTr.transformDirectionAffine(Vector3::UNIT_Y).length(),
                    parentTr.transformDirectionAffine(Vector3::UNIT_Z).length());

                assert(mInheritOrientation ^ mInheritScale);
                mCachedTransform = (mInheritOrientation ? Matrix4::getScale(1.0f / parentScale)  * parentTr : Matrix4::getScale(parentScale)) * tr;
            }
#else
            // Use derived values
            mCachedTransform.makeTransform(
                _getDerivedPosition(),
                _getDerivedScale(),
                _getDerivedOrientation());
#endif
            mCachedTransformOutOfDate = false;
        }
        return mCachedTransform;
    }
    //-----------------------------------------------------------------------
    void Node::_update(bool updateChildren, bool parentHasChanged)
    {
        // always clear information about parent notification
        mParentNotified = false;

        // See if we should process everyone
        if (mNeedParentUpdate || parentHasChanged)
        {
            // Update transforms from parent
            _updateFromParent();
        }

        if(updateChildren)
        {
            if (mNeedChildUpdate || parentHasChanged)
            {
                ChildNodeMap::iterator it, itend;
                itend = mChildren.end();
                for (it = mChildren.begin(); it != itend; ++it)
                {
#if OGRE_NODE_STORAGE_LEGACY
                    Node* child = it->second;
#else
                    Node* child = *it;
#endif
                    child->_update(true, true);
                }
            }
            else
            {
                // Just update selected children
                ChildUpdateSet::iterator it, itend;
                itend = mChildrenToUpdate.end();
                for(it = mChildrenToUpdate.begin(); it != itend; ++it)
                {
                    Node* child = *it;
                    child->_update(true, false);
                }

            }

            mChildrenToUpdate.clear();
            mNeedChildUpdate = false;
        }
    }
    //-----------------------------------------------------------------------
    void Node::_updateFromParent(void) const
    {
        updateFromParentImpl();

        // Call listener (note, this method only called if there's something to do)
        if (mListener)
        {
            mListener->nodeUpdated(this);
        }
    }
    //-----------------------------------------------------------------------
    void Node::updateFromParentImpl(void) const
    {
        mCachedTransformOutOfDate = true;

        if (mParent)
        {
#if OGRE_NODE_INHERIT_TRANSFORM
            // Decompose full transform to position, orientation and scale, shear is lost here.
            _getFullTransform().decomposition(mDerivedPosition, mDerivedScale, mDerivedOrientation);
#else
            // Update orientation
            const Quaternion& parentOrientation = mParent->_getDerivedOrientation();
            if (mInheritOrientation)
            {
                // Combine orientation with that of parent
                mDerivedOrientation = parentOrientation * mOrientation;
            }
            else
            {
                // No inheritance
                mDerivedOrientation = mOrientation;
            }

            // Update scale
            const Vector3& parentScale = mParent->_getDerivedScale();
            if (mInheritScale)
            {
                // Scale own position by parent scale, NB just combine
                // as equivalent axes, no shearing
                mDerivedScale = parentScale * mScale;
            }
            else
            {
                // No inheritance
                mDerivedScale = mScale;
            }

            // Change position vector based on parent's orientation & scale
            mDerivedPosition = parentOrientation * (parentScale * mPosition);

            // Add altered position vector to parents
            mDerivedPosition += mParent->_getDerivedPosition();
#endif
        }
        else
        {
            // Root node, no parent
            mDerivedOrientation = mOrientation;
            mDerivedPosition = mPosition;
            mDerivedScale = mScale;
        }

        mNeedParentUpdate = false;

    }
    //-----------------------------------------------------------------------
    Node* Node::createChild(const Vector3& inTranslate, const Quaternion& inRotate)
    {
        Node* newNode = createChildImpl();
        newNode->setPosition(inTranslate);
        newNode->setOrientation(inRotate);
        this->addChild(newNode);

        return newNode;
    }
    //-----------------------------------------------------------------------
    Node* Node::createChild(const String& name, const Vector3& inTranslate, const Quaternion& inRotate)
    {
        OgreAssert(!name.empty(), "name must not be empty");
        Node* newNode = createChildImpl(name);
        newNode->setPosition(inTranslate);
        newNode->setOrientation(inRotate);
        this->addChild(newNode);

        return newNode;
    }
    //-----------------------------------------------------------------------
    void Node::addChild(Node* child)
    {
        if (child->mParent)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                "Node '" + child->getName() + "' already was a child of '" +
                child->mParent->getName() + "'.",
                "Node::addChild");
        }

#if OGRE_NODE_STORAGE_LEGACY
        mChildren.insert(ChildNodeMap::value_type(child->getName(), child));
#else
        mChildren.push_back(child);
#endif
        child->setParent(this);

    }
    //-----------------------------------------------------------------------
    Node* Node::getChild(unsigned short index) const
    {
        if( index < mChildren.size() )
        {
#if OGRE_NODE_STORAGE_LEGACY
            ChildNodeMap::const_iterator i = mChildren.begin();
            while (index--) ++i;
            return i->second;
#else
            return mChildren[index];
#endif
        }
        else
            return NULL;
    }
    //-----------------------------------------------------------------------
    Node* Node::removeChild(unsigned short index)
    {
        if (index < mChildren.size())
        {
            ChildNodeMap::iterator i = mChildren.begin();
#if OGRE_NODE_STORAGE_LEGACY
            while (index--) ++i;
            Node* ret = i->second;
#else
            i += index;
            Node* ret = *i;
#endif
            // cancel any pending update
            cancelUpdate(ret);

#if OGRE_NODE_STORAGE_LEGACY
            mChildren.erase(i);
#else
            std::swap(*i, mChildren.back());
            mChildren.pop_back();
#endif
            ret->setParent(NULL);
            return ret;
        }
        else
        {
            OGRE_EXCEPT(
                Exception::ERR_INVALIDPARAMS,
                "Child index out of bounds.",
                "Node::getChild" );
        }
        return 0;
    }
    //-----------------------------------------------------------------------
    Node* Node::removeChild(Node* child)
    {
        if (child)
        {
#if OGRE_NODE_STORAGE_LEGACY
            ChildNodeMap::iterator i = mChildren.find(child->getName());
            // ensure it's our child
            if (i != mChildren.end() && i->second == child)
#else
            ChildNodeMap::iterator i = std::find(mChildren.begin(), mChildren.end(), child);
            if(i != mChildren.end() && *i == child)
#endif
            {
                // cancel any pending update
                cancelUpdate(child);

#if OGRE_NODE_STORAGE_LEGACY
                mChildren.erase(i);
#else
                std::swap(*i, mChildren.back());
                mChildren.pop_back();
#endif
                child->setParent(NULL);
            }
        }
        return child;
    }

    //-----------------------------------------------------------------------
    void Node::setOrientation( const Quaternion & q )
    {
        OgreAssertDbg(!q.isNaN(), "Invalid orientation supplied as parameter");
        mOrientation = q;
        mOrientation.normalise();
        needUpdate();
    }
    //-----------------------------------------------------------------------
    void Node::setOrientation( Real w, Real x, Real y, Real z)
    {
        setOrientation(Quaternion(w, x, y, z));
    }
    //-----------------------------------------------------------------------
    void Node::resetOrientation(void)
    {
        mOrientation = Quaternion::IDENTITY;
        needUpdate();
    }

    //-----------------------------------------------------------------------
    void Node::setPosition(const Vector3& pos)
    {
        assert(!pos.isNaN() && "Invalid vector supplied as parameter");
        mPosition = pos;
        needUpdate();
    }


    //-----------------------------------------------------------------------
    void Node::setPosition(Real x, Real y, Real z)
    {
        Vector3 v(x,y,z);
        setPosition(v);
    }

    //-----------------------------------------------------------------------
    Matrix3 Node::getLocalAxes(void) const
    {
        Vector3 axisX = Vector3::UNIT_X;
        Vector3 axisY = Vector3::UNIT_Y;
        Vector3 axisZ = Vector3::UNIT_Z;

        axisX = mOrientation * axisX;
        axisY = mOrientation * axisY;
        axisZ = mOrientation * axisZ;

        return Matrix3(axisX.x, axisY.x, axisZ.x,
                       axisX.y, axisY.y, axisZ.y,
                       axisX.z, axisY.z, axisZ.z);
    }

    //-----------------------------------------------------------------------
    void Node::translate(const Vector3& d, TransformSpace relativeTo)
    {
        switch(relativeTo)
        {
        case TS_LOCAL:
            // position is relative to parent so transform downwards
            mPosition += mOrientation * d;
            break;
        case TS_WORLD:
            // position is relative to parent so transform upwards
            if (mParent)
            {
                mPosition += mParent->convertWorldToLocalDirection(d, true);
            }
            else
            {
                mPosition += d;
            }
            break;
        case TS_PARENT:
            mPosition += d;
            break;
        }
        needUpdate();

    }
    //-----------------------------------------------------------------------
    void Node::translate(Real x, Real y, Real z, TransformSpace relativeTo)
    {
        Vector3 v(x,y,z);
        translate(v, relativeTo);
    }
    //-----------------------------------------------------------------------
    void Node::translate(const Matrix3& axes, const Vector3& move, TransformSpace relativeTo)
    {
        Vector3 derived = axes * move;
        translate(derived, relativeTo);
    }
    //-----------------------------------------------------------------------
    void Node::translate(const Matrix3& axes, Real x, Real y, Real z, TransformSpace relativeTo)
    {
        Vector3 d(x,y,z);
        translate(axes,d,relativeTo);
    }
    //-----------------------------------------------------------------------
    void Node::roll(const Radian& angle, TransformSpace relativeTo)
    {
        rotate(Vector3::UNIT_Z, angle, relativeTo);
    }
    //-----------------------------------------------------------------------
    void Node::pitch(const Radian& angle, TransformSpace relativeTo)
    {
        rotate(Vector3::UNIT_X, angle, relativeTo);
    }
    //-----------------------------------------------------------------------
    void Node::yaw(const Radian& angle, TransformSpace relativeTo)
    {
        rotate(Vector3::UNIT_Y, angle, relativeTo);

    }
    //-----------------------------------------------------------------------
    void Node::rotate(const Vector3& axis, const Radian& angle, TransformSpace relativeTo)
    {
        Quaternion q;
        q.FromAngleAxis(angle,axis);
        rotate(q, relativeTo);
    }

    //-----------------------------------------------------------------------
    void Node::rotate(const Quaternion& q, TransformSpace relativeTo)
    {
        switch(relativeTo)
        {
        case TS_PARENT:
            // Rotations are normally relative to local axes, transform up
            mOrientation = q * mOrientation;
            break;
        case TS_WORLD:
            // Rotations are normally relative to local axes, transform up
            mOrientation = mOrientation * _getDerivedOrientation().Inverse()
                * q * _getDerivedOrientation();
            break;
        case TS_LOCAL:
            // Note the order of the mult, i.e. q comes after
            mOrientation = mOrientation * q;
            break;
        }

        // Normalise quaternion to avoid drift
        mOrientation.normalise();

        needUpdate();
    }

    
    //-----------------------------------------------------------------------
    void Node::_setDerivedPosition( const Vector3& pos )
    {
        //find where the node would end up in parent's local space
        if(mParent)
            setPosition( mParent->convertWorldToLocalPosition( pos ) );
    }
    //-----------------------------------------------------------------------
    void Node::_setDerivedOrientation( const Quaternion& q )
    {
        //find where the node would end up in parent's local space
        if(mParent)
            setOrientation( mParent->convertWorldToLocalOrientation( q ) );
    }

    //-----------------------------------------------------------------------
    const Quaternion & Node::_getDerivedOrientation(void) const
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }
        return mDerivedOrientation;
    }
    //-----------------------------------------------------------------------
    const Vector3 & Node::_getDerivedPosition(void) const
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }
        return mDerivedPosition;
    }
    //-----------------------------------------------------------------------
    const Vector3 & Node::_getDerivedScale(void) const
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }
        return mDerivedScale;
    }
    //-----------------------------------------------------------------------
    Vector3 Node::convertWorldToLocalPosition( const Vector3 &worldPos )
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }
#if OGRE_NODE_INHERIT_TRANSFORM
        return _getFullTransform().inverseAffine().transformAffine(worldPos);
#else
        return mDerivedOrientation.Inverse() * (worldPos - mDerivedPosition) / mDerivedScale;
#endif
    }
    //-----------------------------------------------------------------------
    Vector3 Node::convertLocalToWorldPosition( const Vector3 &localPos )
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }
        return _getFullTransform().transformAffine(localPos);
    }
    //-----------------------------------------------------------------------
    Vector3 Node::convertWorldToLocalDirection( const Vector3 &worldDir, bool useScale )
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }

        return useScale ? 
#if OGRE_NODE_INHERIT_TRANSFORM
            _getFullTransform().inverseAffine().transformDirectionAffine(worldDir) :
            mDerivedOrientation.Inverse() * worldDir;
#else
            mDerivedOrientation.Inverse() * worldDir / mDerivedScale :
            mDerivedOrientation.Inverse() * worldDir;
#endif
    }
    //-----------------------------------------------------------------------
    Vector3 Node::convertLocalToWorldDirection( const Vector3 &localDir, bool useScale )
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }
        return useScale ? 
            _getFullTransform().transformDirectionAffine(localDir) :
            mDerivedOrientation * localDir;
    }
    //-----------------------------------------------------------------------
    Quaternion Node::convertWorldToLocalOrientation( const Quaternion &worldOrientation )
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }
        return mDerivedOrientation.Inverse() * worldOrientation;
    }
    //-----------------------------------------------------------------------
    Quaternion Node::convertLocalToWorldOrientation( const Quaternion &localOrientation )
    {
        if (mNeedParentUpdate)
        {
            _updateFromParent();
        }
        return mDerivedOrientation * localOrientation;

    }
    //-----------------------------------------------------------------------
    void Node::removeAllChildren(void)
    {
        ChildNodeMap::iterator i, iend;
        iend = mChildren.end();
        for (i = mChildren.begin(); i != iend; ++i)
        {
#if OGRE_NODE_STORAGE_LEGACY
            i->second->setParent(0);
#else
            (*i)->setParent(0);
#endif
        }
        mChildren.clear();
        mChildrenToUpdate.clear();
    }
    //-----------------------------------------------------------------------
    void Node::setScale(const Vector3& inScale)
    {
        assert(!inScale.isNaN() && "Invalid vector supplied as parameter");
        mScale = inScale;
        needUpdate();
    }
    //-----------------------------------------------------------------------
    void Node::setScale(Real x, Real y, Real z)
    {
        setScale(Vector3(x, y, z));
    }

    //-----------------------------------------------------------------------
    void Node::setInheritOrientation(bool inherit)
    {
        mInheritOrientation = inherit;
        needUpdate();
    }
    //-----------------------------------------------------------------------
    void Node::setInheritScale(bool inherit)
    {
        mInheritScale = inherit;
        needUpdate();
    }
    //-----------------------------------------------------------------------
    void Node::scale(const Vector3& inScale)
    {
        mScale = mScale * inScale;
        needUpdate();

    }
    //-----------------------------------------------------------------------
    void Node::scale(Real x, Real y, Real z)
    {
        mScale.x *= x;
        mScale.y *= y;
        mScale.z *= z;
        needUpdate();

    }
    //-----------------------------------------------------------------------
    void Node::setInitialState(void)
    {
        mInitialPosition = mPosition;
        mInitialOrientation = mOrientation;
        mInitialScale = mScale;
    }
    //-----------------------------------------------------------------------
    void Node::resetToInitialState(void)
    {
        mPosition = mInitialPosition;
        mOrientation = mInitialOrientation;
        mScale = mInitialScale;

        needUpdate();
    }
    //-----------------------------------------------------------------------
    struct NodeNameExists {
        const String& name;
        bool operator()(const Node* mo) {
            return mo->getName() == name;
        }
    };
    Node* Node::getChild(const String& name) const
    {
#if OGRE_NODE_STORAGE_LEGACY
        ChildNodeMap::const_iterator i = mChildren.find(name);
#else
        NodeNameExists pred = {name};
        ChildNodeMap::const_iterator i = std::find_if(mChildren.begin(), mChildren.end(), pred);
#endif

        if (i == mChildren.end())
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Child node named " + name +
                " does not exist.", "Node::getChild");
        }
#if OGRE_NODE_STORAGE_LEGACY
        return i->second;
#else
        return *i;
#endif
    }
    //-----------------------------------------------------------------------
    Node* Node::removeChild(const String& name)
    {
#if OGRE_NODE_STORAGE_LEGACY
        ChildNodeMap::iterator i = mChildren.find(name);
#else
        OgreAssert(!name.empty(), "name must not be empty");
        NodeNameExists pred = {name};
        ChildNodeMap::iterator i = std::find_if(mChildren.begin(), mChildren.end(), pred);
#endif
        if (i == mChildren.end())
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Child node named " + name +
                " does not exist.", "Node::removeChild");
        }

#if OGRE_NODE_STORAGE_LEGACY
        Node* ret = i->second;
#else
        Node* ret = *i;
#endif
        // Cancel any pending update
        cancelUpdate(ret);

#if OGRE_NODE_STORAGE_LEGACY
        mChildren.erase(i);
#else
        std::swap(*i, mChildren.back());
        mChildren.pop_back();
#endif
        ret->setParent(NULL);

        return ret;


    }
    //-----------------------------------------------------------------------
    Node::ChildNodeIterator Node::getChildIterator(void)
    {
        return ChildNodeIterator(mChildren.begin(), mChildren.end());
    }
    //-----------------------------------------------------------------------
    Node::ConstChildNodeIterator Node::getChildIterator(void) const
    {
        return ConstChildNodeIterator(mChildren.begin(), mChildren.end());
    }
    //-----------------------------------------------------------------------
    Real Node::getSquaredViewDepth(const Camera* cam) const
    {
        Vector3 diff = _getDerivedPosition() - cam->getDerivedPosition();

        // NB use squared length rather than real depth to avoid square root
        return diff.squaredLength();
    }
    //-----------------------------------------------------------------------
    void Node::needUpdate(bool forceParentUpdate)
    {

        mNeedParentUpdate = true;
        mNeedChildUpdate = true;
        mCachedTransformOutOfDate = true;

        // Make sure we're not root and parent hasn't been notified before
        if (mParent && (!mParentNotified || forceParentUpdate))
        {
            mParent->requestUpdate(this, forceParentUpdate);
            mParentNotified = true ;
        }

        // all children will be updated
        mChildrenToUpdate.clear();
    }
    //-----------------------------------------------------------------------
    void Node::requestUpdate(Node* child, bool forceParentUpdate)
    {
        // If we're already going to update everything this doesn't matter
        if (mNeedChildUpdate)
        {
            return;
        }

        mChildrenToUpdate.insert(child);
        // Request selective update of me, if we didn't do it before
        if (mParent && (!mParentNotified || forceParentUpdate))
        {
            mParent->requestUpdate(this, forceParentUpdate);
            mParentNotified = true ;
        }

    }
    //-----------------------------------------------------------------------
    void Node::cancelUpdate(Node* child)
    {
        mChildrenToUpdate.erase(child);

        // Propagate this up if we're done
        if (mChildrenToUpdate.empty() && mParent && !mNeedChildUpdate)
        {
            mParent->cancelUpdate(this);
            mParentNotified = false ;
        }
    }
    //-----------------------------------------------------------------------
    void Node::queueNeedUpdate(Node* n)
    {
        // Don't queue the node more than once
        if (!n->mQueuedForUpdate)
        {
            n->mQueuedForUpdate = true;
            msQueuedUpdates.push_back(n);
        }
    }
    //-----------------------------------------------------------------------
    void Node::processQueuedUpdates(void)
    {
        for (QueuedUpdates::iterator i = msQueuedUpdates.begin();
            i != msQueuedUpdates.end(); ++i)
        {
            // Update, and force parent update since chances are we've ended
            // up with some mixed state in there due to re-entrancy
            Node* n = *i;
            n->mQueuedForUpdate = false;
            n->needUpdate(true);
        }
        msQueuedUpdates.clear();
    }
    //---------------------------------------------------------------------
    Node::DebugRenderable* Node::getDebugRenderable(Real scaling)
    {
        if (!mDebug)
        {
            mDebug = OGRE_NEW DebugRenderable(this);
        }
        mDebug->setScaling(scaling);
        return mDebug;
    }
    //---------------------------------------------------------------------
    //-----------------------------------------------------------------------
    Node::DebugRenderable::DebugRenderable(Node* parent)
        : mParent(parent)
    {
        String matName = "Ogre/Debug/AxesMat";
        mMat = MaterialManager::getSingleton().getByName(matName, ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
        if (!mMat)
        {
            mMat = MaterialManager::getSingleton().create(matName, ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
            Pass* p = mMat->getTechnique(0)->getPass(0);
            p->setLightingEnabled(false);
            p->setPolygonModeOverrideable(false);
            p->setVertexColourTracking(TVC_AMBIENT);
            p->setSceneBlending(SBT_TRANSPARENT_ALPHA);
            p->setCullingMode(CULL_NONE);
            p->setDepthWriteEnabled(false);
        }

        String meshName = "Ogre/Debug/AxesMesh";
        mMeshPtr = MeshManager::getSingleton().getByName(meshName, ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
        if (!mMeshPtr)
        {
            ManualObject mo("tmp");
            mo.begin(mMat->getName());
            /* 3 axes, each made up of 2 of these (base plane = XY)
             *   .------------|\
             *   '------------|/
             */
            mo.estimateVertexCount(7 * 2 * 3);
            mo.estimateIndexCount(3 * 2 * 3);
            Quaternion quat[6];
            ColourValue col[3];

            // x-axis
            quat[0] = Quaternion::IDENTITY;
            quat[1].FromAxes(Vector3::UNIT_X, Vector3::NEGATIVE_UNIT_Z, Vector3::UNIT_Y);
            col[0] = ColourValue::Red;
            col[0].a = 0.8;
            // y-axis
            quat[2].FromAxes(Vector3::UNIT_Y, Vector3::NEGATIVE_UNIT_X, Vector3::UNIT_Z);
            quat[3].FromAxes(Vector3::UNIT_Y, Vector3::UNIT_Z, Vector3::UNIT_X);
            col[1] = ColourValue::Green;
            col[1].a = 0.8;
            // z-axis
            quat[4].FromAxes(Vector3::UNIT_Z, Vector3::UNIT_Y, Vector3::NEGATIVE_UNIT_X);
            quat[5].FromAxes(Vector3::UNIT_Z, Vector3::UNIT_X, Vector3::UNIT_Y);
            col[2] = ColourValue::Blue;
            col[2].a = 0.8;

            Vector3 basepos[7] = 
            {
                // stalk
                Vector3(0, 0.05, 0), 
                Vector3(0, -0.05, 0),
                Vector3(0.7, -0.05, 0),
                Vector3(0.7, 0.05, 0),
                // head
                Vector3(0.7, -0.15, 0),
                Vector3(1, 0, 0),
                Vector3(0.7, 0.15, 0)
            };


            // vertices
            // 6 arrows
            for (size_t i = 0; i < 6; ++i)
            {
                // 7 points
                for (size_t p = 0; p < 7; ++p)
                {
                    Vector3 pos = quat[i] * basepos[p];
                    mo.position(pos);
                    mo.colour(col[i / 2]);
                }
            }

            // indices
            // 6 arrows
            for (uint32 i = 0; i < 6; ++i)
            {
                uint32 base = i * 7;
                mo.triangle(base + 0, base + 1, base + 2);
                mo.triangle(base + 0, base + 2, base + 3);
                mo.triangle(base + 4, base + 5, base + 6);
            }

            mo.end();

            mMeshPtr = mo.convertToMesh(meshName, ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);

        }

    }
    //---------------------------------------------------------------------
    Node::DebugRenderable::~DebugRenderable()
    {
    }
    //-----------------------------------------------------------------------
    const MaterialPtr& Node::DebugRenderable::getMaterial(void) const
    {
        return mMat;
    }
    //---------------------------------------------------------------------
    void Node::DebugRenderable::getRenderOperation(RenderOperation& op)
    {
        return mMeshPtr->getSubMesh(0)->_getRenderOperation(op);
    }
    //-----------------------------------------------------------------------
    void Node::DebugRenderable::getWorldTransforms(Matrix4* xform) const
    {
        // Assumes up to date
        *xform = mParent->_getFullTransform();
        if (!Math::RealEqual(mScaling, 1.0))
        {
            Matrix4 m = Matrix4::IDENTITY;
            Vector3 s(mScaling, mScaling, mScaling);
            m.setScale(s);
            *xform = (*xform) * m;
        }
    }
    //-----------------------------------------------------------------------
    Real Node::DebugRenderable::getSquaredViewDepth(const Camera* cam) const
    {
        return mParent->getSquaredViewDepth(cam);
    }
    //-----------------------------------------------------------------------
    const LightList& Node::DebugRenderable::getLights(void) const
    {
        // Nodes should not be lit by the scene, this will not get called
        static LightList ll;
        return ll;
    }

}

