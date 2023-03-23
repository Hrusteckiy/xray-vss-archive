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
inline Ray2::Ray2 ()
{
    // no initialization for efficiency
}
//----------------------------------------------------------------------------
inline Vector2& Ray2::Origin ()
{
    return m_kOrigin;
}
//----------------------------------------------------------------------------
inline const Vector2& Ray2::Origin () const
{
    return m_kOrigin;
}
//----------------------------------------------------------------------------
inline Vector2& Ray2::Direction ()
{
    return m_kDirection;
}
//----------------------------------------------------------------------------
inline const Vector2& Ray2::Direction () const
{
    return m_kDirection;
}
//----------------------------------------------------------------------------


