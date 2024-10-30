#include "Drawing.h"
#include "Hook.h"

ULONG drawCallCount = 0;
BOOL Drawing::bInit = FALSE; // Status of the initialization of ImGui.
bool Drawing::bDisplay = true; // Status of the menu display.
bool Drawing::bSetPos = false; // Status to update ImGui window size / position.
ImVec2 Drawing::vWindowPos = { 0, 0 }; // Last ImGui window position.
ImVec2 Drawing::vWindowSize = { 0, 0 }; // Last ImGui window size.

DWORD AvailableVRAM;
ULONG textureCount = 0;

DWORD GetVRAMUsage(IDirect3DDevice9* pDevice) {
	DWORD availableMemory = pDevice->GetAvailableTextureMem();
	return (DWORD)(availableMemory / (1024 * 1024)); // Convert bytes to MB
}

/**
    @brief : Hook of the EndScene function.
    @param  D3D9Device : Current Direct3D9 Device Object.
    @retval : Result of the original EndScene function.
**/
HRESULT Drawing::hkEndScene(const LPDIRECT3DDEVICE9 D3D9Device)
{
	if (GetAsyncKeyState(VK_END) & 1)
	{
		Hook::UnHookDirectX();
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, Hook::hDDLModule, 0, nullptr);
		return Hook::oEndScene(D3D9Device);
	}

	return Hook::oEndScene(D3D9Device);
}

/**
    @brief : function that init ImGui for rendering.
    @param pDevice : Current Direct3D9 Device Object given by the hooked function.
**/
void Drawing::InitImGui(const LPDIRECT3DDEVICE9 pDevice)
{
	AvailableVRAM = GetVRAMUsage(pDevice);

	D3DDEVICE_CREATION_PARAMETERS CP;
	pDevice->GetCreationParameters(&CP);
	Hook::window = CP.hFocusWindow;
	Hook::HookWindow();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Hook::window);
	ImGui_ImplDX9_Init(pDevice);

	bInit = TRUE;
}

HRESULT Drawing::hkDrawPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	drawCallCount++;
	return Hook::oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT Drawing::hkCreateTexture(IDirect3DDevice9* pDevice, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) {
	HRESULT result = Hook::oCreateTexture(pDevice, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);

	if (SUCCEEDED(result)) {
		textureCount++;
	}

	return result;
}


ULONG Drawing::hkRelease(IUnknown* pResource)
{
	if (pResource != nullptr) {
		IUnknown* tempResource = nullptr;
		if (SUCCEEDED(pResource->QueryInterface(__uuidof(IDirect3DBaseTexture9), (void**)&tempResource))) {
			D3DRESOURCETYPE type = ((IDirect3DBaseTexture9*)tempResource)->GetType();
			if (type == D3DRTYPE_TEXTURE) {
				textureCount--;
			}
			tempResource->Release(); // Release the interface acquired by QueryInterface
		}
	}


	return Hook::oRelease(pResource);
}

HRESULT Drawing::hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	if (!Hook::pDevice)
		Hook::pDevice = pDevice;

	if (!bInit)
		InitImGui(pDevice);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDisplay = !bDisplay;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bDisplay)
	{

		DWORD vramUsage = GetVRAMUsage(pDevice);

		ImGui::Begin("Byond ImGui", &bDisplay);
		{
			ImGui::SetWindowSize({ 500, 300 }, ImGuiCond_Once);

			if (vWindowPos.x != 0.0f && vWindowPos.y != 0.0f && vWindowSize.x != 0.0f && vWindowSize.y != 0.0f && bSetPos)
			{
				ImGui::SetWindowPos(vWindowPos);
				ImGui::SetWindowSize(vWindowSize);
				bSetPos = false;
			}

			if (bSetPos == false)
			{
				vWindowPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
				vWindowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
			}

			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::Text("Particle Draw Calls: %lu", drawCallCount);
			ImGui::Text("VRAM Usage: %lu MB / %lu MB", AvailableVRAM - vramUsage, AvailableVRAM);
			ImGui::Text("Texture Count: %lu", textureCount);


			/*
			ImGui::Text("Memory Usage: %.2f MB", ImGui::GetIO().MemoryUse / 1024.0f / 1024.0f);
			ImGui::Text("Draw Calls: %d", ImGui::GetIO().DrawCalls);
			ImGui::Text("Vertices: %d", ImGui::GetIO().Vertices);
			ImGui::Text("Textures: %d", ImGui::GetIO().Textures);
			ImGui::Text("Texture Memory: %.2f MB", ImGui::GetIO().TextureMemory / 1024.0f / 1024.0f);
			ImGui::Text("Active Shaders: %d", ImGui::GetIO().ActiveShaders);
			*/




		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	drawCallCount = 0;

	return Hook::oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
