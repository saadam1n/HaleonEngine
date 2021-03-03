#include "D3D12Sync.h"
#include "../Kernel/Haleon.h"

namespace Haleon {

	void Synchronizer::Synchronize(ID3D12CommandQueue* Queue, DWORD Milliseconds) {
		FenceValue++;
		Queue->Signal(FencePoint, FenceValue);
		if (FencePoint->GetCompletedValue() < FenceValue) {
			FencePoint->SetEventOnCompletion(FenceValue, FenceEvent);
			DWORD FenceResult = WaitForSingleObject(FenceEvent, Milliseconds);
#ifdef HALEON_DEBUG
			if (FenceResult == WAIT_OBJECT_0) {
				ResetEvent(FenceEvent);
				// Success!
				return;
			} else {
				// Uh oh, error handling time
				exit(EXIT_FAILURE);
			}
#endif
		}
	}

}