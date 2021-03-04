#ifndef HALEON_GRAPHICS_BACKEND_OPENGL3_CORE_RENDERER_H
#define HALEON_GRAPHICS_BACKEND_OPENGL3_CORE_RENDERER_H

#include "D3D9GraphicsWindow.h"
#include "../../Frame.h"
#include <d3d9.h>

namespace Haleon {

	// TODO: create BaseCoreRenderer object
	class CoreRenderer : public BaseObject {
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

		// The Direct3D9 device used for rendering 
		IDirect3DDevice9* Device;

		// The window that the renderer was creating using. 
		GraphicsWindow* WindowContext;
		// The current frame associated with the renderer. StartFrame and EndFrame simply copy the values from this memeber variable to the specified memeory address
		Frame CurrentFrame;

	};

}

#endif

#ifndef HALEON_GRAPHICS_API_DIRECT3D9
//#error Please select D3D9 in Cmake to include this file
#endif