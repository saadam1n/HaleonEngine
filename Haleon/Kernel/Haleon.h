// Copyright 2021 fuzdex
#ifndef HALEON_KERNEL_CORE_H
#define HALEON_KERNEL_CORE_H

#define COFFEE_API

void* operator new(size_t bytes);
void operator delete(void* addr, size_t bytes);

#define HALEON_ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

namespace Haleon {

	struct AllocationInfo {
		size_t Allocated;
		size_t Freed;
	};

	//extern AllocationInfo GlobalAllocationInfo, thread_local ThreadAllocationInfo;

	void Initialize(void);
	void Terminate(void);

}

#endif