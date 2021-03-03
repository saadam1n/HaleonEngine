#include "PhotonGraphicsWindow.h"
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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		// GPU-supported double buffering is still a nice-to-have. I have also noticed that it's much faster
		// SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, false);
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