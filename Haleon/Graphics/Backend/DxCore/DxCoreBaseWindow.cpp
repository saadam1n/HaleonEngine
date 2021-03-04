#include "DxCoreBaseWindow.h"

namespace Haleon {

	uint32_t DxCoreBaseWindow::GetAPIFlags(void) {
		return 0;
	}

	void DxCoreBaseWindow::CreateGraphics(void) {
		SDL_VERSION(&DriverInfo.version)
			SDL_GetWindowWMInfo(Window, &DriverInfo);
	}

	void DxCoreBaseWindow::DestroyGraphics(void) {

	}

	HWND DxCoreBaseWindow::GetWindowHandleWin32(void) {
		return DriverInfo.info.win.window;
	}

}