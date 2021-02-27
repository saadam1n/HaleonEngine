#include "GLGraphicsWindow.h"
#include <GL/glew.h>

namespace Haleon {

	void LoadOpenGL(void) {
		static bool Loaded = false;
		if (!Loaded) {
			glewExperimental = true;
			glewInit();
			Loaded = true;
		}
	}

	uint32_t GraphicsWindow::GetAPIFlags(void) {
		return SDL_WINDOW_OPENGL;
	}

	void GraphicsWindow::CreateGraphics(void)  {
		Context = SDL_GL_CreateContext(Window);
		LoadOpenGL();
	}

	void GraphicsWindow::DestroyGraphics(void) {
		SDL_GL_DeleteContext(Window);
	}

}