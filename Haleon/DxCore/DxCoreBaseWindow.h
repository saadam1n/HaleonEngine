#ifndef HALEON_DXCORE_BASE_WINDOW_H
#define HALEON_DXCORE_BASE_WINDOW_H

#include "../Window/Window.h"
#include <SDL_syswm.h>
#include <Windows.h>

namespace Haleon {

	class DxCoreBaseWindow : public BaseWindow {
	protected:
		friend class CoreRenderer;

		virtual uint32_t GetAPIFlags(void) final override;
		virtual void CreateGraphics(void)  final override;
		virtual void DestroyGraphics(void) final override;

		HWND GetWindowHandleWin32(void);

		SDL_SysWMinfo DriverInfo;
	};

}

#endif