#include "D3D12SwapChain.h"
#include <directx/d3dx12.h>

namespace Haleon {

	void SwapChainWrapper::CreateResources(DeviceWrapper Device) {
		D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapRTVDesc;
		DescriptorHeapRTVDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		DescriptorHeapRTVDesc.NumDescriptors = HALEON_DIRECT3D12_SWAP_CHAIN_BUFFER_COUNT;
		DescriptorHeapRTVDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DescriptorHeapRTVDesc.NodeMask = Device.GetNodeMask();
		Device->CreateDescriptorHeap(&DescriptorHeapRTVDesc, IID_PPV_ARGS(&DescriptorHeapRTV));

		CD3DX12_CPU_DESCRIPTOR_HANDLE BufferRTV(DescriptorHeapRTV->GetCPUDescriptorHandleForHeapStart());
		for (UINT BufferIndex = 0; BufferIndex < HALEON_DIRECT3D12_SWAP_CHAIN_BUFFER_COUNT; BufferIndex++) {
			ID3D12Resource** CurrentBuffer = Buffers + BufferIndex;
			SwapChain->GetBuffer(BufferIndex, IID_PPV_ARGS(CurrentBuffer));
			Device->CreateRenderTargetView(*CurrentBuffer, NULL, BufferRTV);
			BufferRTV.Offset(1, Device.GetDescriptorSizes()->RTV);
		}

		CurrentBackBufferIndex = 0;
	}

	void SwapChainWrapper::SelectNextBuffer(void) {
		CurrentBackBufferIndex++;
		CurrentBackBufferIndex %= HALEON_DIRECT3D12_SWAP_CHAIN_BUFFER_COUNT;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE SwapChainWrapper::GetCurrentBackBufferRTV(UINT OffsetSize) {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeapRTV->GetCPUDescriptorHandleForHeapStart(), CurrentBackBufferIndex, OffsetSize);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE SwapChainWrapper::GetCurrentBackBufferRTV(DeviceWrapper Device) {
		return GetCurrentBackBufferRTV(Device.GetDescriptorSizes()->RTV);
	}

	ID3D12Resource* SwapChainWrapper::GetCurrentBackBufferResource(void) {
		return Buffers[CurrentBackBufferIndex];
	}

}