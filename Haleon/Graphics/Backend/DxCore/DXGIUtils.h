#ifndef HALEON_GRAPHICS_BACKEND_DXCORE_DXGIUTILS_H
#define HALEON_GRAPHICS_BACKEND_DXCORE_DXGIUTILS_H

#include <dxgi.h>
#include "DxCoreBaseWindow.h"

namespace Haleon {

	class SwapChainBase {
	public:
		/* 
		This function fills in some of the missing information in SwapChainDesc
		This includes:
		- Window handle and fullscreen state
		- Width and height
		- Format and monitor refresh rate
		*/
		void Create(IDXGIFactory* Factory, IUnknown* Device, DxCoreBaseWindow* Window, IDXGIAdapter* Monitor, DXGI_SWAP_CHAIN_DESC* SwapChainDesc);
		// Free all resources using Release()
		void Free(void);
	protected:
		// The swap chain
		IDXGISwapChain* SwapChain;

		/*
		Fills the information of Mode
		*/
		void FillModeDesc(DXGI_MODE_DESC* Mode);
	};

}

#endif