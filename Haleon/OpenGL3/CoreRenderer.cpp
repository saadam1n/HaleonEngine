#include "CoreRenderer.h"
#include <GL/glew.h>

namespace Haleon {

	void CoreRenderer::Create(GraphicsWindow* Window) {
		WindowContext = Window;

		/*
		Set clear values
		There is a small confusion regarding how this actually works
		glClear clears the buffer(s) with a certain value
		This value is set by glClear(Color/Depth/Stencil)
		So you only need to set them after application startup
		*/
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClearStencil(0xFF);

		// Set up basic construction stuff
		CurrentFrame.ID = 0;
		CurrentFrame.Success = false;
	}
	
	void CoreRenderer::Free(void) {

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
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void CoreRenderer::SwapBuffers(void) {
		SDL_GL_SwapWindow(WindowContext->Window);
	}

}