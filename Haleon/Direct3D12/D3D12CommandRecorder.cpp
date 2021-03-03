#include "D3D12CommandRecorder.h"

namespace Haleon {

	void CommandRecorder::Clear(void) {
		CommandList->Reset(CommandAllocator, NULL);
	}

	void CommandRecorder::Begin(void) {
		CommandAllocator->Reset();
		Clear();
	}

	void CommandRecorder::End(void) {
		CommandList->Close();
	}

	ID3D12GraphicsCommandList* CommandRecorder::operator->(void) {
		return operator ID3D12GraphicsCommandList*();
	}

	CommandRecorder::operator ID3D12GraphicsCommandList* (void) {
		return CommandList;
	}

}