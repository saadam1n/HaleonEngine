#ifndef HALEON_DIRECT3D12_COMMAND_RECORDER_H
#define HALEON_DIRECT3D12_COMMAND_RECORDER_H

#include <directx/d3d12.h>

namespace Haleon {

	class CommandRecorder {
	public:
		// Empty all of the contents of the command recorder
		void Clear(void);

		// Begin and End sound a lot like fixed function stuff, I might want to replace it with some other name
		// Clear the contents of the command list and begin a new list
		void Begin(void);
		// End the command recording and close the list
		void End(void);

		// Operator overloading to use it like a normal command list
		ID3D12GraphicsCommandList* operator->(void);
		// Useful when getting the address of a value
		operator ID3D12GraphicsCommandList* (void);
	protected:
		friend class CoreRenderer;
		ID3D12CommandAllocator* CommandAllocator;
		ID3D12GraphicsCommandList* CommandList;
	};

}

#endif
