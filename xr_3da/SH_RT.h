#ifndef SH_RT_H
#define SH_RT_H

class	ENGINE_API	CRT
{
public:
	IDirect3DTexture8*	pSurface;
	IDirect3DSurface8*	pRT;
	CTexture*			pTexture;
	
	DWORD				dwReference;
	DWORD				dwWidth;
	DWORD				dwHeight;

	CRT					(){	ZeroMemory(this,sizeof(CRT));}

	void				Create	(LPCSTR Name, DWORD w, DWORD h);
	void				Destroy	();
};

#endif // SH_RT_H