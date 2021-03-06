#include "D3D12DeviceWrapper.h"

namespace Haleon {

	ID3D12Device* DeviceWrapper::operator->(void) {
		return operator ID3D12Device * ();
	}

	DeviceWrapper::operator ID3D12Device* (void) {
		return Device;
	}

	DescriptorSizeInfo* DeviceWrapper::GetDescriptorSizes(void) {
		return &DescriptorSizes;
	}

	UINT DeviceWrapper::GetNodeMask(void) {
		return NodeMask;
	}

	ID3D12Device** DeviceWrapper::GetInternalDevice(void) {
		return &Device;
	}

}