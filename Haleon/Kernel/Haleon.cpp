// Copyright 2021 fuzdex
// Haleon
#include "Haleon.h"
#include "../Utility/Thread.h"
#include "../IO/Log.h"
// Simple DirectMedia Layer 2
#include <SDL_video.h>
// Standard C++ Library
#include <string>
// Standard C library
#include <stdlib.h>
#include <assert.h>

namespace Haleon {
	//AllocationInfo GlobalAllocationInfo, thread_local ThreadAllocationInfo;
}

static size_t AllocatedBytesGlobal = 0;
thread_local static size_t AllocatedBytesThread = 0;

static size_t FreedByesGlobal = 0;
thread_local static size_t FreedByesThread = 0;

void* operator new(size_t bytes) {
	void* addr = malloc(bytes);
	assert(addr);
	AllocatedBytesGlobal += bytes;
	AllocatedBytesThread += bytes;
	return addr;
}

void operator delete(void* addr, size_t bytes) {
	assert(addr);
	FreedByesGlobal += bytes;
	FreedByesThread += bytes;
	free(addr);
}

namespace Haleon {

	extern void GraphicsBackendInitialize(void);
	extern void GraphicsBackendTerminate(void);

	void Initialize(void) {
		// Main thread in this case refers to the thread Haleon was initialized with
		CurrentThreadInfo.Debug.SetName("Main");

		if (SDL_VideoInit(nullptr) == -1) {
			using namespace std::literals;
			DebugLog0("Video subsystem was unable initialize. SDL2 error: "s << SDL_GetError() << LogFlush);
			abort();
		}

		GraphicsBackendInitialize();

		DebugLog1(COFFEE_SEVERITY_INFO << "Haleon has initialized successfully" << LogFlush);
	}

	void Terminate(void) {
		GraphicsBackendTerminate();
		SDL_VideoQuit();
		DebugLog1(COFFEE_SEVERITY_INFO << "Haleon has terminated successfully" << LogFlush);
	}

}