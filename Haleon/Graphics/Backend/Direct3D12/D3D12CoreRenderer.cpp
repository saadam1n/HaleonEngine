#include "D3D12CoreRenderer.h"
#include <iostream>

namespace Haleon {

	void CoreRenderer::Create(GraphicsWindow* Window) {
		// Enable D3D12 debugging on debug builds
#ifdef HALEON_DEBUG
		ID3D12Debug* Debug;
		D3D12GetDebugInterface(IID_PPV_ARGS(&Debug));
		Debug->EnableDebugLayer();
#endif

		UINT AdapterNodeMask = 0;
		UINT MonitorModeCount = 1;
		// TODO: Automatically choose the format based on what's avaible
		DXGI_FORMAT SwapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		// We will be needing information about the monitor and the graphics card
		IDXGIFactory* Factory;
		IDXGIAdapter* Adapter;
		IDXGIOutput* Output;

		// Use default adapter and output
		CreateDXGIFactory(IID_PPV_ARGS(&Factory));
		Factory->EnumAdapters(AdapterNodeMask, &Adapter);
		Adapter->EnumOutputs(1, &Output);
		// Get the refresh rate of the monitor, which is needed to set up the swap chain
		DXGI_MODE_DESC MonitorMode;;
		Output->GetDisplayModeList(SwapChainFormat, DXGI_ENUM_MODES_INTERLACED | DXGI_ENUM_MODES_SCALING, &MonitorModeCount, &MonitorMode);


		// Initialized the device. I'm not using very new features so feature level 11.0 works
		D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Device.GetInternalDevice()));
		Device.NodeMask = AdapterNodeMask;
		Device.DescriptorSizes.RTV         = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV        );
		Device.DescriptorSizes.DSV         = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV        );
		Device.DescriptorSizes.CBV_SRV_UAV = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		
		// Initialize the command queue
		D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
		CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		CommandQueueDesc.NodeMask = Device.GetNodeMask();
		Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(CommandQueue.GetInternalCommandQueue()));

		// Initialize the things we need to start recording commands
		CreateCommandRecorder(&CommandList, Device.GetNodeMask());

		// Set up the fence
		CreateFence(&CommandFence);

		// Create the swap chain
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		//ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
		// Bind the swap chain to the window
		SwapChainDesc.OutputWindow = Window->GetWindowHandleWin32();
		SwapChainDesc.Windowed = TRUE; // TODO: add a method to the window base class to tell whether the window is windowed or fullscreen
		// Disable MSAA (for now)
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		// Set up window color buffer description
		SwapChainDesc.BufferDesc.Width = Window->CurrentState.Size.x;
		SwapChainDesc.BufferDesc.Height = Window->CurrentState.Size.y;
		SwapChainDesc.BufferDesc.Format = SwapChainFormat;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		// TODO: retreive correct refresh rate
		SwapChainDesc.BufferDesc.RefreshRate = MonitorMode.RefreshRate;
		// Other information about the buffers
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = HALEON_DIRECT3D12_BUFFER_COUNT;
		// Set swap effect and flags
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		SwapChain.Create(Factory, CommandQueue, Window, Adapter, &SwapChainDesc);
		SwapChain.CreateResources(Device);
		
		D3D12_VIEWPORT Viewport;
		Viewport.TopLeftX = 0.0f;
		Viewport.TopLeftY = 0.0f;
		Viewport.Width =  (float)SwapChainDesc.BufferDesc.Width;
		Viewport.Height = (float)SwapChainDesc.BufferDesc.Height;
		Viewport.MinDepth = 0.0f;
		Viewport.MaxDepth = 1.0f;
		CommandList->RSSetViewports(1, &Viewport);
		CommandList.End();
		CommandQueue.Execute(&CommandList);
		CommandFence.Synchronize(CommandQueue);

		Output->Release();
		Adapter->Release();
		Factory->Release();
		
	}

	void CoreRenderer::CreateFence(Synchronizer* Fence) {
		Fence->FenceValue = 0   ;
		Fence->FencePoint = NULL;
		Fence->FenceEvent = NULL;

		Device->CreateFence(Fence->FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence->FencePoint));
		Fence->FenceEvent = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
	}

	void CoreRenderer::FreeFence(Synchronizer* Fence) {
		CloseHandle(Fence->FenceEvent);
		Fence->FencePoint->Release();
	}

	void CoreRenderer::CreateCommandRecorder(CommandRecorder* CommandRecorder, UINT NodeMask) {
		// Initialize the command allocator
		Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandRecorder->CommandAllocator));
		// Create the command list using the command allocator and the command queue nodemask
		Device->CreateCommandList(NodeMask, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandRecorder->CommandAllocator, NULL, IID_PPV_ARGS(&CommandRecorder->CommandList));
	}

	// Free a command recorder
	void CoreRenderer::FreeCommandRecorder(CommandRecorder* CommandRecorder) {
		CommandRecorder->CommandList->Release();
		CommandRecorder->CommandAllocator->Release();
	}

	void CoreRenderer::Free(void) {
		// Make sure the GPU is down executing all commands before freeing any resources. In a way this prevents a resource hazard that arises from synch issues
		CommandFence.Synchronize(CommandQueue);

		// Free swap chain resources 
		SwapChain.DescriptorHeapRTV->Release();
		for (UINT Index = 0; Index < HALEON_DIRECT3D12_BUFFER_COUNT; Index++) {
			SwapChain.Buffers[Index]->Release();
		}
		SwapChain.SwapChainBase::SwapChain->Release();

		// Free command exec compoenents
		FreeFence(&CommandFence);
		FreeCommandRecorder(&CommandList);

		// Free core components
		CommandQueue->Release();
		Device->Release();
	}

	void CoreRenderer::StartFrame(Frame* NewFrame) {
		// Reset the command list
		CommandList.Begin();

		// Clear the buffer
		CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(SwapChain.GetCurrentBackBufferResource(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
		ClearBuffer();

		CurrentFrame.ID++;
		*NewFrame = CurrentFrame;
	}
	
	void CoreRenderer::EndFrame(Frame* CurrentFrame) {
		CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(SwapChain.GetCurrentBackBufferResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		CommandList.End();
		CommandQueue.Execute(&CommandList);
		CommandFence.Synchronize(CommandQueue);
		SwapBuffers();

		SwapChain.SelectNextBuffer();

		// Until I get a more advanced system that can detect runtime errors and recover, it's either true or exit(-1) for now
		this->CurrentFrame.Success = true;
		*CurrentFrame = this->CurrentFrame;
	}

	void CoreRenderer::ClearBuffer(void) {
		const static float Color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		
		CommandList->ClearRenderTargetView(SwapChain.GetCurrentBackBufferRTV(Device), Color, 0, NULL);
	}

	void CoreRenderer::SwapBuffers(void) {
		SwapChain.SwapChain->Present(1, 0);
	}

}