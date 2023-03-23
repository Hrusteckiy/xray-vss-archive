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

//----------------------------------------------------------------------------
inline Box2::Box2 ()
{
    // no initialization for efficiency
}
//----------------------------------------------------------------------------
inline Vector2& Box2::Center ()
{
    return m_kCenter;
}
//----------------------------------------------------------------------------
inline const Vector2& Box2::Center () const
{
    return m_kCenter;
}
//----------------------------------------------------------------------------
inline Vector2& Box2::Axis (int i)
{
    assert( 0 <= i && i < 2 );
    return m_akAxis[i];
}
//----------------------------------------------------------------------------
inline const Vector2& Box2::Axis (int i) const
{
    assert( 0 <= i && i < 2 );
    return m_akAxis[i];
}
//----------------------------------------------------------------------------
inline Vector2* Box2::Axes ()
{
    return m_akAxis;
}
//----------------------------------------------------------------------------
inline const Vector2* Box2::Axes () const
{
    return m_akAxis;
}
//----------------------------------------------------------------------------
inline Real& Box2::Extent (int i)
{
    assert( 0 <= i && i < 2 );
    return m_afExtent[i];
}
//----------------------------------------------------------------------------
inline const Real& Box2::Extent (int i) const
{
    assert( 0 <= i && i < 2 );
    return m_afExtent[i];
}
//----------------------------------------------------------------------------
inline Real* Box2::Extents ()
{
    return m_afExtent;
}
//----------------------------------------------------------------------------
inline const Real* Box2::Extents () const
{
    return m_afExtent;
}
//----------------------------------------------------------------------------


