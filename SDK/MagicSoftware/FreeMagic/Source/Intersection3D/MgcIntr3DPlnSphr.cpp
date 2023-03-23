// Magic Software, Inc.
// http://www.magic-software.com
// Copyright (c) 2000-2002.  All Rights Reserved
//
// Source code from Magic Software is supplied under the terms of a license
// agreement and may not be copied or disclosed except in accordance with the
// terms of that agreement.  The various license agreements may be found at
// the Magic Software web site.  This file is subject to the license
//
// FREE SOURCE CODE
// http://www.magic-software.com/License/free.pdf

#include "MgcIntr3DPlnSphr.h"
using namespace Mgc;

//----------------------------------------------------------------------------
bool Mgc::TestIntersection (const Plane& rkPlane, const Sphere& rkSphere,
    bool bUnitNormal)
{
    Vector3 kNormal = rkPlane.Normal();
    Real fConstant = rkPlane.Constant();
    if ( !bUnitNormal )
    {
        Real fLength = kNormal.Unitize();
        fConstant /= fLength;
    }

    Real fPseudoDistance = kNormal.Dot(rkSphere.Center()) - fConstant;
    return Math::FAbs(fPseudoDistance) <= rkSphere.Radius();
}
//----------------------------------------------------------------------------
bool Mgc::Culled (const Plane& rkPlane, const Sphere& rkSphere,
    bool bUnitNormal)
{
    Vector3 kNormal = rkPlane.Normal();
    Real fConstant = rkPlane.Constant();
    if ( !bUnitNormal )
    {
        Real fLength = kNormal.Unitize();
        fConstant /= fLength;
    }

    Real fTmp = kNormal.Dot(rkSphere.Center()) - fConstant;
    return fTmp <= -rkSphere.Radius();
}
//----------------------------------------------------------------------------


