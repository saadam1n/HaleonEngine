#include "VkCoreRenderer.h"
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>

namespace Haleon {

	VkInstance Instance;

	// Later I want to initialize everything myself (similiarly to how I need to clean up DXGI stuff) so I can have more control over things
	void GraphicsBackendInitialize(void) {
		
	}

	void GraphicsBackendTerminate(void) {

	}

	void CoreRenderer::Create(GraphicsWindow* Window) {

	}

	void CoreRenderer::Free(void) {

	}

}