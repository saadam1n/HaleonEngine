#ifndef HALEON_DIRECT3D11_CORE_RENDERER
#define HALEON_DIRECT3D11_CORE_RENDERER

#include "D3D11GraphicsWindow.h"
#include "../Graphics/Frame.h"
#include <d3d11.h>
#include <dxgi.h>

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

	private:
		// Clear the currently bound buffer in the swap chain
		void ClearBuffer(void);
		// Swap the buffers in the swap chain
		void SwapBuffers(void);

		// The Direct3D11 device used for rendering 
		ID3D11Device* Device;
		ID3D11DeviceContext* DeviceContext;
		// Swap chain
		IDXGISwapChain* SwapChain;
		ID3D11Texture2D* BackBuffer;
		ID3D11RenderTargetView* BackBufferRTV;

		// The window that the renderer was creating using. 
		GraphicsWindow* WindowContext;
		// The current frame associated with the renderer. StartFrame and EndFrame simply copy the values from this memeber variable to the specified memeory address
		Frame CurrentFrame;
	};

}

#endif