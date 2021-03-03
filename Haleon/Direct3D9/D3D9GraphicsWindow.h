#ifndef HALEON_DIRECT3D9_GRAPHICS_WINDOW_H
#define HALEON_DIRECT3D9_GRAPHICS_WINDOW_H

#include "../Window/Window.h"
#include "../DxCore/DxCoreBaseWindow.h"
#include <SDL_syswm.h>

namespace Haleon {

	class GraphicsWindow : public DxCoreBaseWindow {
	protected:
		friend class CoreRenderer;
	};

}

#endif

#ifndef HALEON_GRAPHICS_API_DIRECT3D9
#error Please select D3D9 in Cmake to include this file
#endif