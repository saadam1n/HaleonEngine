#ifndef HALEON_DIRECT3D9_GRAPHICS_WINDOW_H
#define HALEON_DIRECT3D9_GRAPHICS_WINDOW_H

#include "../Window/Window.h"
#include <SDL_syswm.h>

namespace Haleon {

	class GraphicsWindow : public BaseWindow {
	protected:
		friend class CoreRenderer;

		virtual uint32_t GetAPIFlags(void) final override;
		virtual void CreateGraphics(void)  final override;
		virtual void DestroyGraphics(void) final override;

		SDL_SysWMinfo DriverInfo;
	};

}

#endif

#ifndef HALEON_GRAPHICS_API_DIRECT3D9
#error Please select D3D9 in Cmake to include this file
#endif