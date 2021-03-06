#ifndef HALEON_GRAPHICS_BACKEND_DIRECT3D12_COMMAND_EXEC_H
#define HALEON_GRAPHICS_BACKEND_DIRECT3D12_COMMAND_EXEC_H

#include <directx/d3d12.h>

namespace Haleon {

	class CommandRecorder;

	class CommandExecutor {
	public:
		void Execute(CommandRecorder* Recorder);

		// Operator overloading to use it like a normal command queue
		ID3D12CommandQueue* operator->(void);
		// Useful when getting the address of a value
		operator ID3D12CommandQueue* (void);

		ID3D12CommandQueue** GetInternalCommandQueue(void);
	protected:
		ID3D12CommandQueue* CommandQueue;
	};

}

#endif