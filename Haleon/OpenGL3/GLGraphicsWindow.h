// Copyright 2021 fuzdex
#ifndef HALEON_OPENGL3_GRAPHICS_WINDOW_H
#define HALEON_OPENGL3_GRAPHICS_WINDOW_H

#include "../Window/Window.h"

namespace Haleon {

	class GraphicsWindow : public BaseWindow {
	protected:
		friend class CoreRenderer;

		virtual uint32_t GetAPIFlags(void) final override;
		virtual void CreateGraphics(void)  final override;
		virtual void DestroyGraphics(void) final override;

		SDL_GLContext Context;
	};

}

#endif

#ifndef HALEON_GRAPHICS_API_OPENGL3
#error Please select OpenGL3 in cmake to include this file
#endif