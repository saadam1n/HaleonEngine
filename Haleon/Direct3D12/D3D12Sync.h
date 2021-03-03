#ifndef HALEON_DIRECT3D12_SYNC_H
#define HALEON_DIRECT3D12_SYNC_H

#include <directx/d3d12.h>

namespace Haleon {

	class Synchronizer {
	public:
		void Synchronize(ID3D12CommandQueue* Queue, DWORD Milliseconds = 1000);
	protected:
		friend class CoreRenderer;
		// The fence interface
		ID3D12Fence* FencePoint;
		// The fence value
		UINT64 FenceValue;
		// The event used to wait for completion of the fence
		HANDLE FenceEvent;
	};

}

#endif