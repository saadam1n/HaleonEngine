#ifndef HALEON_GRAPHOCS_BACKEND_DIRECT3D12_DEVICE_WRAPPER_H
#define HALEON_GRAPHOCS_BACKEND_DIRECT3D12_DEVICE_WRAPPER_H

#include <directx/d3d12.h>

namespace Haleon {

	struct DescriptorSizeInfo {
		UINT RTV;
		UINT DSV;
		UINT CBV_SRV_UAV;
	};

	class DeviceWrapper {
	public:
		ID3D12Device* operator->(void);
		operator ID3D12Device* (void);

		DescriptorSizeInfo* GetDescriptorSizes(void);
		UINT GetNodeMask(void);

		ID3D12Device** GetInternalDevice(void);
	protected:
		friend class CoreRenderer;
		ID3D12Device* Device;

		DescriptorSizeInfo DescriptorSizes;
		UINT NodeMask;
	};

}

#endif