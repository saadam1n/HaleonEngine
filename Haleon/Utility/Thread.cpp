// Copyright 2021 fuzdex
#include "Thread.h"
#include <thread>
#include <sstream>
#include <iomanip>

namespace Haleon {

	thread_local ThreadInfo CurrentThreadInfo;
	static uint64_t ThreadCounter = 0;

	std::string GetLoggingFormat(const std::string& name) {
		std::ostringstream StringBuilder;
		StringBuilder << '[' << name << "/%s]:  \t";
		return StringBuilder.str();
	}

	ThreadInfo::ThreadID::ThreadID(void) : 
		Local(ThreadCounter++),
		Global((uint64_t)std::hash<std::thread::id>{ }(std::this_thread::get_id()))
	{/* EMPTY CONSTRUCTOR */}

	ThreadInfo::ThreadDebug::ThreadDebug(const std::string& name, const ThreadID& ID) : Name(name), LogSource(GetLoggingFormat(Name))
	{/* EMPTY CONSTRUCTOR */}

	void ThreadInfo::ThreadDebug::SetName(const std::string& name) {
		Name = name;
		LogSource = GetLoggingFormat(Name);
	}

	const std::string& ThreadInfo::ThreadDebug::GetName(void) {
		return Name;
	}

	const std::string& ThreadInfo::ThreadDebug::GetLoggingName(void) {
		return LogSource;
	}

	ThreadInfo::ThreadInfo(const std::string& name) : ID(), Debug(name, ID) 
	{/* EMPTY CONSTRUCTOR */}

}