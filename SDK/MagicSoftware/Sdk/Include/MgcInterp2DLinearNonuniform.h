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

#ifndef MGCINTERP2DLINEARNONUNIFORM_H
#define MGCINTERP2DLINEARNONUNIFORM_H

// Linear interpolation of a network of triangles whose vertices are of the
// form (x,y,f(x,y)).

#include "MgcDelaunay2D.h"

namespace Mgc {


class MAGICFM Interp2DLinearNonuniform : public Delaunay2D
{
public:
    // Construction and destruction.
    //
    // The first constructor implicitly creates the triangle network from the
    // input vertices.  This constructor accepts ownership of the input arrays
    // and will delete them during destruction.  The underlying triangle
    // network object also will be deleted.
    //
    // The second constructor shares the input triangle network.  This
    // constructor accepts ownership of the input function array, but does
    // not delete the triangle network on destruction.  The idea is that the
    // network was shared, either from an explicitly created one by the
    // application or from one created by another interpolator.

    Interp2DLinearNonuniform (int iVertexQuantity, Vector2* akVertex,
        Real* afF);

    Interp2DLinearNonuniform (Delaunay2D& rkNet, Real* afF);

    virtual ~Interp2DLinearNonuniform ();

    // Linear interpolation.  The return value is 'true' if and only if the
    // input point is in the convex hull of the input vertices, in which case
    // the interpolation is valid.
    bool Evaluate (const Vector2& rkPoint, Real& rfF);

protected:
    Real* m_afF;
};

} // namespace Mgc

#endif


