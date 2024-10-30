#ifndef HOOK_H
#define HOOK_H

#include "pch.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
using tEndScene =		HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice);
using tPresent =		HRESULT(APIENTRY*)(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
using tReset =			HRESULT(APIENTRY*)(D3DPRESENT_PARAMETERS* pPresentationParameters);
using tDrawPrimitive =	HRESULT(APIENTRY*)(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
using tCreateTexture =	HRESULT(APIENTRY*)(IDirect3DDevice9* pDevice, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);
using tRelease =		ULONG(APIENTRY*)(IUnknown* pResource);

class Hook
{
public:
	static IDirect3DDevice9* pDevice;
	static tEndScene oEndScene;
	static tPresent oPresent;
	static tDrawPrimitive oDrawPrimitive;
	static tCreateTexture oCreateTexture;
	static tRelease oRelease;
	static HWND window;
	static HMODULE hDDLModule;

	static void HookDirectX();
	static void UnHookDirectX();
	static void HookWindow();
	static void UnHookWindow();

private:
	static int windowHeight, windowWidth;
	static void* d3d9Device[119];
	static WNDPROC OWndProc;
	static tReset oReset;


	static BOOL CALLBACK enumWind(HWND handle, LPARAM lp);
	static HWND GetProcessWindow();
	static BOOL GetD3D9Device(void** pTable, size_t size);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static HRESULT APIENTRY hkReset(D3DPRESENT_PARAMETERS* pPresentationParameters);
};

#endif
