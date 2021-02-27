#include "D3D9GraphicsWindow.h"

namespace Haleon {

	uint32_t GraphicsWindow::GetAPIFlags(void) {
		return 0;
	}

	void GraphicsWindow::CreateGraphics(void) {
		SDL_VERSION(&DriverInfo.version)
		SDL_GetWindowWMInfo(Window, &DriverInfo);
	}

	void GraphicsWindow::DestroyGraphics(void) {

	}

}