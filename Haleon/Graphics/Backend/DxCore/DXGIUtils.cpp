#include "DXGIUtils.h"

namespace Haleon {

	void SwapChainBase::Create(IDXGIFactory* Factory, IUnknown* Device, DxCoreBaseWindow* Window, IDXGIAdapter* Monitor, DXGI_SWAP_CHAIN_DESC* SwapChainDesc) {
#ifdef HALEON_DEBUG
		SwapChain = nullptr;
#endif
		
		// Set up window information
		SwapChainDesc->OutputWindow = Window->GetWindowHandleWin32();
		SwapChainDesc->Windowed     = TRUE;

		// Set swap chain size
		SwapChainDesc->BufferDesc.Width  = Window->CurrentState.Size.x;
		SwapChainDesc->BufferDesc.Height = Window->CurrentState.Size.y;

		FillModeDesc(&SwapChainDesc->BufferDesc);
		
		Factory->CreateSwapChain(Device, SwapChainDesc, &SwapChain);
	}

	void SwapChainBase::Free(void) {
		SwapChain->Release();
	}

	void SwapChainBase::FillModeDesc(DXGI_MODE_DESC* Mode) {
		// Monitors use 32 bit color and support approximately 60Hz. I will need to find a better solution than this later.
		Mode->Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
		Mode->RefreshRate.Numerator   = 60;
		Mode->RefreshRate.Denominator = 1;
	}

}