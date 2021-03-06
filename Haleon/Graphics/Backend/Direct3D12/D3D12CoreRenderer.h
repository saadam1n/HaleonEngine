#ifndef HALEON_GRAPHICS_BACKEND_DIRECT3D12_CORE_RENDERER_H
#define HALEON_GRAPHICS_BACKEND_DIRECT3D12_CORE_RENDERER_H

#include "D3D12GraphicsWindow.h"
#include "../../Frame.h"
#include <directx/d3dx12.h>
#include <dxgi.h>
#include "D3D12Sync.h"
#include "D3D12CommandRecorder.h"
#include "D3D12DeviceWrapper.h"
#include "D3D12SwapChain.h"
#include "D3D12CommandExec.h"

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

		// The D3D12 Device
		DeviceWrapper Device;
		// The command queue. For simplicity I only use one global command queue for the entire renderer
		CommandExecutor CommandQueue;
		// The command list
		CommandRecorder CommandList;
		// The fence object we use for sync.
		Synchronizer CommandFence;

		// Swap chain resources.
		SwapChainWrapper SwapChain;

		// The window that the renderer was creating using. 
		GraphicsWindow* WindowContext;
		// The current frame associated with the renderer. StartFrame and EndFrame simply copy the values from this memeber variable to the specified memeory address
		Frame CurrentFrame;

	};

}

#endif
