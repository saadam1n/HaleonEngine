#ifndef HALEON_GRAPHICS_BACKEND_DIRECT3D12_SWAP_CHAIN_H
#define HALEON_GRAPHICS_BACKEND_DIRECT3D12_SWAP_CHAIN_H

#include <directx/d3d12.h>
#include "../DxCore/DXGIUtils.h"
#include "D3D12DeviceWrapper.h"

#define HALEON_DIRECT3D12_SWAP_CHAIN_BUFFER_COUNT 2 

namespace Haleon {

	class SwapChainWrapper : public SwapChainBase {
	protected:
		friend class CoreRenderer;
		void CreateResources(DeviceWrapper Device);
		void SelectNextBuffer(void);
		ID3D12DescriptorHeap* DescriptorHeapRTV;
		ID3D12Resource* Buffers[HALEON_DIRECT3D12_SWAP_CHAIN_BUFFER_COUNT];

		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTV(UINT OffsetSize);
		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTV(DeviceWrapper Device);

		ID3D12Resource* GetCurrentBackBufferResource(void);

		UINT CurrentBackBufferIndex;
	};

}

#endif
