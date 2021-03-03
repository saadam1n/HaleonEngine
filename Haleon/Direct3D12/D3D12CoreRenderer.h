#ifndef HALEON_DIRECT3D12_CORE_RENDERER_H
#define HALEON_DIRECT3D12_CORE_RENDERER_H

#include "D3D12GraphicsWindow.h"
#include "../Graphics/Frame.h"
#include <directx/d3dx12.h>
#include <dxgi.h>
#include "D3D12Sync.h"
#include "D3D12CommandRecorder.h"

#define HALEON_DIRECT3D12_BUFFER_COUNT 2 

namespace Haleon {

	class CoreRenderer {
	public:
		// Set up the renderer using the specified window. This is the "constructor" of the renderer.
		void Create(GraphicsWindow* Window);
		// Free any resources that the renderer is using. This does not free any BaseGraphicsObjects, which need to be freed manually 
		void Free(void);

		// Begins a new frame and fills in the information into the frame structure
		void StartFrame(Frame* NewFrame);
		// Ends the current frame and fills in the information into the frame structure
		void EndFrame(Frame* CurrentFrame);

		// I might move these methods out of public

		// Function that creates a fence
		void CreateFence(Synchronizer* Fence);
		// Function that frees a fence
		void FreeFence(Synchronizer* Fence);

		// Create a command recorder
		void CreateCommandRecorder(CommandRecorder* CommandRecorderObject, UINT NodeMask);
		// Free a command recorder
		void FreeCommandRecorder(CommandRecorder* CommandRecorder);

	private:
		// Clear the currently bound buffer in the swap chain
		void ClearBuffer(void);
		// Swap the buffers in the swap chain
		void SwapBuffers(void);

		// Gets the current back buffer
		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTV(void);

		// The D3D12 Device
		ID3D12Device* Device;
		// The command queue. For simplicity I only use one global command queue for the entire renderer
		ID3D12CommandQueue* CommandQueue;
		// The command list
		CommandRecorder CommandList;
		// The fence object we use for sync.
		Synchronizer CommandFence;

		// Swap chain resources. TODO: move this into a class
		IDXGISwapChain* SwapChain;
		ID3D12DescriptorHeap* SwapChainRTVHeap;
		ID3D12Resource* SwapChainBuffers[HALEON_DIRECT3D12_BUFFER_COUNT];
		UINT CurrentBufferIndex;

		// The window that the renderer was creating using. 
		GraphicsWindow* WindowContext;
		// The current frame associated with the renderer. StartFrame and EndFrame simply copy the values from this memeber variable to the specified memeory address
		Frame CurrentFrame;

	};

}

#endif
