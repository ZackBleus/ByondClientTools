#ifndef DRAWING_H
#define DRAWING_H

#include "pch.h"

class Drawing
{
public:
	static bool bDisplay;
	static BOOL bInit;
	static bool bSetPos;

	static HRESULT	APIENTRY hkEndScene(LPDIRECT3DDEVICE9 D3D9Device);
	static HRESULT	APIENTRY hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
	static HRESULT	APIENTRY hkDrawPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	static HRESULT	APIENTRY hkCreateTexture(IDirect3DDevice9* pDevice, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);
	static ULONG	APIENTRY hkRelease(IUnknown* pResource);

private:
	static ImVec2 vWindowPos;
	static ImVec2 vWindowSize;

	static void InitImGui(LPDIRECT3DDEVICE9 pDevice);
};

#endif