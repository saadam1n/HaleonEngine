#ifndef HALEON_OPENGL3_CORE_RENDERER_H
#define HALEON_OPENGL3_CORE_RENDERER_H

#include "PhotonGraphicsWindow.h"
#include "../Graphics/Frame.h"
#include <GL/glew.h>

namespace Haleon {

	struct ColorRGBA {
		unsigned char R, G, B, A;
	};

	struct BackBuffer {
	public:
		BackBuffer(void);
		void Create(uint32_t x, uint32_t y);
		void Create(int32_t x, int32_t y);
		void Free(void);

		ColorRGBA Sample(uint32_t x, uint32_t y);

		void Write(uint32_t x, uint32_t y, ColorRGBA color);
		void Write(uint32_t x, uint32_t y, unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255);
	protected:
		friend class CoreRenderer;

		ColorRGBA* Data;
		uint32_t Width;
		uint32_t Height;

	private:
		uint64_t GetIndex(uint32_t X, uint32_t Y);
	};

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

		// Our backbuffer. There is where we accumulate the color before sending it to the window with glDrawPixels
		BackBuffer BackBuffer;
		// Well actually, glDrawPixels is really slow. Instead I use fixed functionality to draw a textured quad to the screen as a fullscreen pass.
		GLuint ScreenTexture;
		/*
		This is super old OpenGL stuff, but in a nutshell, a display list collects OpenGL commands (sort of like a command buffer/queue) so we can call it later.
		This is useful for drawing a fullscreen textured quad
		I use the name "PresentList" since it is used to present the BackBuffer to the screen
		*/
		GLuint PresentList; 

		// The window that the renderer was creating using. 
		GraphicsWindow* WindowContext;
		// The current frame associated with the renderer. StartFrame and EndFrame simply copy the values from this memeber variable to the specified memeory address
		Frame CurrentFrame;
	};

}

#endif
