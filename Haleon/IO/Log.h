// Copyright 2021 fuzdex
#ifndef HALEON_IO_LOG_H
#define HALEON_IO_LOG_H

// Haleon
#include "../Kernel/Haleon.h"
// OpenGL mathematics
#include <glm/glm.hpp>
// Standard C++ Library
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>

/*
Logging levels
There are 5 levels of logging, starting from 0 (highest priority) to 4 (lowest priority)
*/
// Things like system errors and coffee fatals
#define HALEON_LOG_LEVEL_0
// Things like coffee errors and warns and assertion results
#define HALEON_LOG_LEVEL_1
// Things like important information
#define HALEON_LOG_LEVEL_2
// Things like basic information
#define HALEON_LOG_LEVEL_3
// Things like less useful information
//#define COFFEE_LOG_LEVEL_4


// Remember to add a semicolon!
#define DebugLog(expr) StreamOut << expr

// Is there a better way to do all of this?

#ifdef HALEON_LOG_LEVEL_0
#define DebugLog0 DebugLog
#else
#define DebugLog0(expr) 
#endif

#ifdef HALEON_LOG_LEVEL_1
#define DebugLog1 DebugLog
#else
#define DebugLog1(expr) 
#endif

#ifdef HALEON_LOG_LEVEL_2
#define DebugLog2 DebugLog
#else
#define DebugLog2(expr) 
#endif

#ifdef HALEON_LOG_LEVEL_3
#define DebugLog3 DebugLog
#else
#define DebugLog3(expr) 
#endif

#ifdef HALEON_LOG_LEVEL_4
#define DebugLog4 DebugLog
#else
#define DebugLog4(expr) 
#endif

// Note: logging will be done in a Minecraft syle way. which is something like this:
// [HH:MM:SS] [Thread Name/(INFO/WARN/ERROR/FATAL)]: Message

namespace Haleon {

	class LoggingStream;

	typedef std::function<void(LoggingStream&)> LoggingStreamManipulator;

	extern LoggingStreamManipulator LogFlush;

	enum LogSeverity {
		COFFEE_SEVERITY_INFO = 0,
		COFFEE_SEVERITY_WARN = 1,
		COFFEE_SEVERITY_ERROR = 2,
		COFFEE_SEVERITY_FATAL = 3,
		COFFEE_SEVERITY_ASSERT_SUCCESS = 4,
		COFFEE_SEVERITY_ASSERT_FAILURE = 5
	};

	class Assertion {
	public:
		Assertion(bool res, const std::string& fail, const std::string& success, LogSeverity sf = COFFEE_SEVERITY_ASSERT_FAILURE, LogSeverity ss = COFFEE_SEVERITY_ASSERT_SUCCESS);
		~Assertion(void) = default;
		void ProcessAssertion(void) const;
		operator const std::string& (void) const;
		operator LogSeverity(void) const;
		operator bool(void) const;
	private:
		bool Result;
		std::string Message;
		LogSeverity Severity;
	};

	class LoggingStream {
	public:
		LoggingStream(void);
		~LoggingStream(void);

		void AddOutput(std::ostream& out);
		void RemoveOutput(std::ostream& out);

		void Info(const std::string& msg);
		void Warn(const std::string& msg);
		void Error(const std::string& msg);
		void Fatal(const std::string& msg);
		void SetSeverity(LogSeverity severity);

		// Add output
		LoggingStream& operator<<(std::ostream& out);
		// Remove output
		LoggingStream& operator>>(std::ostream& out);
		// Change severity
		LoggingStream& operator<<(LogSeverity severity);
		// Assert a condition
		LoggingStream& operator<<(const Assertion& assertion);

		// Basic string types
		LoggingStream& operator<<(const std::string& msg);
		LoggingStream& operator<<(const char* msg);

		// Integer types
		LoggingStream& operator<<(const int val);

		// Basic math types
		LoggingStream& operator<<(const glm::ivec2& val);

		LoggingStream& operator<<(const LoggingStreamManipulator& manip);

		void Flush(void);

	private:
		std::stringstream Buffer;
		std::vector<std::ostream*> Outputs;
		LogSeverity CurrentSeverity;
	};

	extern LoggingStream StreamOut;

}

#endif


/*
class Logger {
public:
	Logger(void);
	void Print(const std::string& msg, LogType Type = COFFEE_LOG_TYPE_INFO);
	void Flush(std::ostream& out);
	// Please flush before changing colored text state
	void SetConsoleDebugState(bool autoflush, bool color);

	Logger& operator<<(const std::string& msg);

private:
	void OnPrint(void);
	std::ostringstream Buffer;
	struct {
		bool AutomaticFlush;
		bool ColoredText;
	} ConsoleDebugSettings;
};

extern Logger DefaultLogger;
*/