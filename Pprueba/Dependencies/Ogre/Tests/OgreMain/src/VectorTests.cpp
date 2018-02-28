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
#include <gtest/gtest.h>
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreVector4.h"

using namespace Ogre;

//--------------------------------------------------------------------------
TEST(VectorTests,Vector2Scaler)
{
    EXPECT_EQ(Vector2(1, 1) + Vector2(2, 2), Vector2(3, 3));
    EXPECT_EQ(1 + Vector2(2), Vector2(3, 3));
    Vector2 v1;
    v1 = 1;
    EXPECT_EQ(v1, Vector2(1, 1));
    v1 = 0.0;
    EXPECT_EQ(v1, Vector2::ZERO);
    v1 += 3;
    EXPECT_EQ(v1, Vector2(3, 3));

    v1 = 3 - Vector2(2);
    EXPECT_EQ(v1, Vector2(1));
    v1 = Vector2(5) - 7;
    EXPECT_EQ(v1, Vector2(-2));
    v1 -= 4;
    EXPECT_EQ(v1, Vector2(-6));
}
//--------------------------------------------------------------------------
TEST(VectorTests,Vector3Scaler)
{
    EXPECT_EQ(Vector3(1, 1, 1) + Vector3(2, 2, 2), Vector3(3, 3, 3));
    EXPECT_EQ(1 + Vector3(2), Vector3(3, 3, 3));
    Vector3 v1;
    v1 = 1;
    EXPECT_EQ(v1, Vector3(1));
    v1 = 0.0;
    EXPECT_EQ(v1, Vector3::ZERO);
    v1 += 3;
    EXPECT_EQ(v1, Vector3(3));

    v1 = 3 - Vector3(2);
    EXPECT_EQ(v1, Vector3(1));
    v1 = Vector3(5) - 7;
    EXPECT_EQ(v1, Vector3(-2));
    v1 -= 4;
    EXPECT_EQ(v1, Vector3(-6));
}
//--------------------------------------------------------------------------
TEST(VectorTests,Vector4Scaler)
{
    EXPECT_EQ(Vector4(1, 1, 1, 1) + Vector4(2, 2, 2, 2), Vector4(3, 3, 3, 3));
    EXPECT_EQ(1 + Vector4(2, 2, 2, 2), Vector4(3, 3, 3, 3));
    Vector4 v1;
    v1 = 1;
    EXPECT_EQ(v1, Vector4(1, 1, 1, 1));
    v1 = 0.0;
    EXPECT_EQ(v1, Vector4(0,0,0,0));
    v1 += 3;
    EXPECT_EQ(v1, Vector4(3,3,3,3));

    v1 = 3 - Vector4(2,2,2,2);
    EXPECT_EQ(v1, Vector4(1,1,1,1));
    v1 = Vector4(5,5,5,5) - 7;
    EXPECT_EQ(v1, Vector4(-2,-2,-2,-2));
    v1 -= 4;
    EXPECT_EQ(v1, Vector4(-6,-6,-6,-6));
}

TEST(VectorTests,PerpendicularVector)
{
    Vector3 v1(1, 0, 0);
    Vector3 v2(0, 1, 0);
    Vector3 v3(0, 0, 1);

    EXPECT_EQ(v1.dotProduct(v1.perpendicular()), (Real)0);
    EXPECT_EQ(v2.dotProduct(v2.perpendicular()), (Real)0);
    EXPECT_EQ(v3.dotProduct(v3.perpendicular()), (Real)0);

    EXPECT_EQ((v1 + v2).dotProduct((v1 + v2).perpendicular()), (Real)0);
    EXPECT_EQ((v2 + v3).dotProduct((v2 + v3).perpendicular()), (Real)0);
    EXPECT_EQ((v3 + v1).dotProduct((v3 + v1).perpendicular()), (Real)0);
}

//--------------------------------------------------------------------------
