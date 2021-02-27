#ifndef HALEON_GRAPHICS_FRAME_H
#define HALEON_GRAPHICS_FRAME_H

#include "../Kernel/Object.h"
#include <stdint.h>

namespace Haleon {

	struct Frame : public BaseObject {
	public:
		// Identifier of the frame, represented as a counter that starts at 0 for the first frame
		uint64_t ID;
		// No major runtime errors were encountered while processing the frame
		bool Success;
	};

}

#endif