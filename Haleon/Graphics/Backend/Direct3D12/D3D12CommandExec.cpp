#include "D3D12CommandExec.h"
#include "D3D12CommandRecorder.h"

namespace Haleon {

	void CommandExecutor::Execute(CommandRecorder* Recorder) {
		ID3D12CommandList* ListAddr[1] = { Recorder->CommandList };
		CommandQueue->ExecuteCommandLists(1, ListAddr);
	}

	ID3D12CommandQueue* CommandExecutor::operator->(void) {
		return operator ID3D12CommandQueue * ();
	}

	CommandExecutor::operator ID3D12CommandQueue* (void) {
		return CommandQueue;
	}

	ID3D12CommandQueue** CommandExecutor::GetInternalCommandQueue(void) {
		return &CommandQueue;
	}

}