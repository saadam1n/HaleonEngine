#ifndef HALEON_GRAPHICS_BACKEND_DIRECT3D12_GRAPHICS_WINDOW_H
#define HALEON_GRAPHICS_BACKEND_DIRECT3D12_GRAPHICS_WINDOW_H

#include "../DxCore/DxCoreBaseWindow.h"

namespace Haleon {

	// Could use a typedef but this is better for the future when I may need to add specific things to this class
	class GraphicsWindow : public DxCoreBaseWindow {
	public:
		// TODO: add a function that automatically creates a DXGI_SWAP_CHAIN_DESC from the window information
	protected:
		friend class CoreRenderer;
	};

}

#endif