// Texture.h: interface for the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE_H__0E25CF47_FFEC_11D3_B4E3_4854E82A090D__INCLUDED_)
#define AFX_TEXTURE_H__0E25CF47_FFEC_11D3_B4E3_4854E82A090D__INCLUDED_
#pragma once

// Wizzard pack
extern ENGINE_API IDirect3DBaseTexture9* TWLoader2D
(
		u32&				mem,
		const char *		fname,

		// return values
		D3DFORMAT&			fmt,
		u32&				dwWidth,
		u32&				dwHeight
);

#endif // !defined(AFX_TEXTURE_H__0E25CF47_FFEC_11D3_B4E3_4854E82A090D__INCLUDED_)
