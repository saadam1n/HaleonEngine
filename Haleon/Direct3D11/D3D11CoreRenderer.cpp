#include "D3D11CoreRenderer.h"

namespace Haleon {

	void CoreRenderer::Create(GraphicsWindow* Window) {
		IDXGIFactory* Factory;
		IDXGIAdapter* Adapter;

		CreateDXGIFactory(IID_PPV_ARGS(&Factory));
		Factory->EnumAdapters(0, &Adapter);

		D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		UINT DeviceCreateFlags = 0;
		DeviceCreateFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef HALEON_DEBUG
		DeviceCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D11CreateDevice(Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, DeviceCreateFlags, FeatureLevels, HALEON_ARRAY_SIZE(FeatureLevels), D3D11_SDK_VERSION, &Device, NULL, &DeviceContext);

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		//ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
		// Bind the swap chain to the window
		SwapChainDesc.OutputWindow = Window->GetWindowHandleWin32();
		SwapChainDesc.Windowed = true; // TODO: add a method to the window base class to tell whether the window is windowed or fullscreen
		// Disable MSAA (for now)
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		// Set up window color buffer description
		SwapChainDesc.BufferDesc.Width = Window->CurrentState.Size.x;
		SwapChainDesc.BufferDesc.Height = Window->CurrentState.Size.y;
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		// TODO: retreive correct refresh rate
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		// Other information about the buffers
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 1;
		// Set swap effect and flags
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		Factory->CreateSwapChain(Device, &SwapChainDesc, &SwapChain);

		SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));
		Device->CreateRenderTargetView(BackBuffer, NULL, &BackBufferRTV);

		Adapter->Release();
		Factory->Release();
	}

	void CoreRenderer::Free(void) {
		BackBufferRTV->Release();
		BackBuffer->Release();
		SwapChain->Release();
		DeviceContext->Release();
		Device->Release();
	}

	void CoreRenderer::StartFrame(Frame* NewFrame) {
		ClearBuffer();

		CurrentFrame.ID++;
		*NewFrame = CurrentFrame;
	}

	void CoreRenderer::EndFrame(Frame* CurrentFrame) {
		SwapBuffers();

		// Until I get a more advanced system that can detect runtime errors and recover, it's either true or exit(-1) for now
		this->CurrentFrame.Success = true;
		*CurrentFrame = this->CurrentFrame;
	}

	void CoreRenderer::ClearBuffer(void) {
		const static float Color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		DeviceContext->ClearRenderTargetView(BackBufferRTV, Color);
	}

	void CoreRenderer::SwapBuffers(void) {
		SwapChain->Present(1, 0);
	}

}