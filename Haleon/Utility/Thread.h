// Copyright 2021 fuzdex
#ifndef HALEON_UTILITY_THREAD_H
#define HALEON_UTILITY_THREAD_H

// Standard C++ Library
#include <string>

// Maximum number of digits Coffee supports when printing to the log
#define HALEON_DEBUG_LOG_MAX_THREAD_DIGITS 6

namespace Haleon {
	enum LogSeverity;
	/*
	Contains information about a thread
	*/
	class ThreadInfo {
	public:
		/*
		Contains the numeric ID representations of the thread
		It is not recommended to set any of these values in normal circumstances,
		But it is allowed for ease of debugging in certain cases
		*/
		struct ThreadID {
		public:
			/*
			The ID in the context of the program.
			A global counter starts at 0 and increments at each thread creation.
			The global counter will not be decremented on thread destruction.
			The defaul thread is given the ID of 0.
			*/
			uint64_t Local;
			/*
			The ID in the context of the operating system
			*/
			uint64_t Global;
			ThreadID(void);
		} ID;
		/*
		Contains debug information about the thread
		*/
		struct ThreadDebug {
		public:
			ThreadDebug(const std::string& name, const ThreadID& ID);
			void SetName(const std::string& name);
			const std::string& GetName(void);
		private:
			friend std::string GetThreadLoggingName(LogSeverity type);
			const std::string& GetLoggingName(void);
			/*
			The string representation of the name of the thread
			*/
			std::string Name;
			/*
			The preformatted logging representation of the thread
			*/
			std::string LogSource;
		} Debug;
		ThreadInfo(const std::string& name = "Unnamed thread");
	};

	/*
	Information about the current thread
	*/
	extern thread_local ThreadInfo CurrentThreadInfo;
}

#endif