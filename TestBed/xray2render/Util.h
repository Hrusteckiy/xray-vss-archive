#ifndef UTIL_H
#define UTIL_H

HRESULT LoadVertexShader(LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* fileName, LPDIRECT3DVERTEXSHADER9* pShader);
HRESULT LoadPixelShader(LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* fileName, LPDIRECT3DPIXELSHADER9* pShader);

#endif // UTIL_H;
