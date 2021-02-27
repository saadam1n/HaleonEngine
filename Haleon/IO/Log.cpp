// Copyright 2021 fuzdex
// Haleon
#include "Log.h"
#include "../Utility/Thread.h"
// libconsole
#include <console.h>
// Standard C Library
#include <assert.h>
#include <math.h>
#include <time.h>
// Standard C++ Library
#include <chrono>
#include <iomanip>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#define COLOR_ESCAPE_SEQUENCE '\033'

namespace Haleon {

	void UpdateConsoleColor(Color color) {
		static Color CurrentColor = Color::WHITE;
		// Avoid having to set the console color to the same color
		if (CurrentColor != color) {
			setcolor(color);
			CurrentColor = color;
		}
	}

	void ClearBuffer(std::stringstream& buf) {
		static std::string EmptyString("");
		buf.str(EmptyString);
		buf.clear();
	}

	std::string& GetCurrentTimeString(void) {
		static std::string CurrentTimeString;
		static time_t LastCallTime = 0;
		time_t CurrentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		// Avoid having to create a new time string if one second has not elapsed
		// This way we only create a new one when it is outdated
		double TimeDiff = difftime(CurrentTime, LastCallTime);
		if (TimeDiff >= 1.0) {
			LastCallTime = CurrentTime;
			// I prefer the standard C libarary for time since it is much simpler to get the time than in chrono
			std::tm LocalTime;
			localtime_s(&LocalTime, &CurrentTime);
			std::ostringstream TempStrBuilder;
			TempStrBuilder << '[' << std::put_time(&LocalTime, "%X") << "] ";
			CurrentTimeString = TempStrBuilder.str();
		}
		return CurrentTimeString;
	}

	std::string GetThreadLoggingName(LogSeverity type) {
		static const char* LogTypeStr[] = {
			"INFO",
			"WARN",
			"ERROR",
			"FATAL",
			"ASSERT", // SUCCESS
			"ASSERT"  // FAILURE
		};
		// Each thread has it's own printing buffer
		// This way I don't need to have a mutex
		// This also deallocates memory on thread destruction
		thread_local static std::unique_ptr<char> PrintBuffer(new char[1024]);
		// Set null terminator at 0 to start from beginning
		PrintBuffer.get()[0] = '\0';
		std::sprintf(PrintBuffer.get(), CurrentThreadInfo.Debug.GetLoggingName().c_str(), LogTypeStr[type]);
		return std::string(PrintBuffer.get());
	}

	LoggingStream::LoggingStream(void) : CurrentSeverity(COFFEE_SEVERITY_INFO) {
		setcolor(Color::WHITE);
	}

	LoggingStream::~LoggingStream(void) {
		Flush();
	}

	void LoggingStream::AddOutput(std::ostream& out) {
		std::ostream* addr_out = &out;
		// Check to make sure a stream was not added twice
		// Realistically there is no reason for a stream to be added twice
		if (std::find(Outputs.begin(), Outputs.end(), addr_out) != Outputs.end()) {
			DebugLog0(COFFEE_SEVERITY_ERROR << "Output stream was added twice to a vector" << LogFlush);
		}
		Outputs.push_back(addr_out);
	}

	void LoggingStream::RemoveOutput(std::ostream& out) {
		std::ostream* addr_out = &out;
		for (auto Itr = Outputs.begin(); Itr != Outputs.end(); Itr++) {
			if (*Itr == addr_out) {
				Outputs.erase(Itr);
				// Break here since it is unlikely there are extras of it that somehow made it into the vector of outputs
				break;
			}
		}
	}

	void LoggingStream::Info(const std::string& msg) {
		*this << COFFEE_SEVERITY_INFO << msg;
	}

	void LoggingStream::Warn(const std::string& msg) {
		*this << COFFEE_SEVERITY_WARN << msg;
	}

	void LoggingStream::Error(const std::string& msg) {
		*this << COFFEE_SEVERITY_ERROR << msg;
	}

	void LoggingStream::Fatal(const std::string& msg) {
		*this << COFFEE_SEVERITY_FATAL << msg;
	}

	void LoggingStream::SetSeverity(LogSeverity severity) {
		const static Color ConsoleColor[] = {
			Color::WHITE,
			Color::YELLOW,
			Color::RED,
			Color::RED,
			Color::GREEN,
			Color::RED
		};
		UpdateConsoleColor(ConsoleColor[(int)severity]);
		CurrentSeverity = severity;
	}

	void LoggingStream::Flush(void) {
		// Get size of string to avoid flush operations for empty buffers
		Buffer.seekp(0, std::ios::end);
		std::stringstream::pos_type Size = Buffer.tellp();
		// Early return if there is nothing in the string
		if (Size == 0) {
			return;
		}
		// Automatically append new line
		Buffer << std::endl;
		std::string LogString(Buffer.str());
		// Print string for each output
		for (std::ostream* Output : Outputs) {
			*Output << LogString;
		}
		// Clear the buffer
		ClearBuffer(Buffer);
	}

	LoggingStream& LoggingStream::operator<<(std::ostream& out) {
		AddOutput(out);
		return *this;
	}

	LoggingStream& LoggingStream::operator>>(std::ostream& out) {
		RemoveOutput(out);
		return *this;
	}

	LoggingStream& LoggingStream::operator<<(LogSeverity severity) {
		// Make sure buffer is empty
		Flush();
		SetSeverity(severity);
		// Append beginning of log message
		Buffer << GetCurrentTimeString() << GetThreadLoggingName(CurrentSeverity);
		return *this;
	}

	LoggingStream& LoggingStream::operator<<(const std::string& msg) {
		Buffer << msg;
		return *this;
	}

	LoggingStream& LoggingStream::operator<<(const char* msg) {
		Buffer << msg;
		return *this;
	}

	LoggingStream& LoggingStream::operator<<(const int val) {
		Buffer << val;
		return *this;
	}

	LoggingStream& LoggingStream::operator<<(const glm::ivec2& val) {
		Buffer << '(' << val.x << ", " << val.y << ')';
		return *this;
	}

	LoggingStream& LoggingStream::operator<<(const Assertion& assertion) {
		*this << assertion.operator Haleon::LogSeverity() << assertion.operator const std::string&();
		Flush();
		assertion.ProcessAssertion();
		return *this;
	}

	LoggingStream& LoggingStream::operator<<(const LoggingStreamManipulator& manip) {
		manip(*this);
		return *this;
	}

	LoggingStream StreamOut;

	void LogFlushFunc(LoggingStream& stream) {
		stream.Flush();
	}

	LoggingStreamManipulator LogFlush = LogFlushFunc;

	Assertion::Assertion(bool res, const std::string& fail, const std::string& success, LogSeverity sf, LogSeverity ss) : 
		Result(res), 
		Message(res ? success : fail),
		Severity(res ? ss : sf)
	{ /* EMPTY CONSTRUCTOR */ }

	Assertion::operator const std::string& (void) const {
		return Message;
	}

	Assertion::operator LogSeverity(void) const {
		return Severity;
	}

	Assertion::operator bool(void) const {
		return Result;
	}

	void Assertion::ProcessAssertion(void) const {
		if (!Result) {
			// Throw exception if assertion did not eval to true
			throw std::runtime_error(Message);
		}
	}

}




// Old logging code, do not use

/*	
Logger::Logger(void) {
	SetConsoleDebugState(true, true);
}

/*
I primarily base my logging style off the one Minecraft uses
This means the the format is along the lines of:
[HH:MM:SS] [Thread Name/Log Type]: Message
[HH:MM:SS] is the local time (note that hours is in the range [0, 24], not [0, 12]. This gets rid of the need to use AM or PM in the log somewhere
Thread name is the name of the calling thread
Log Type is the type of log the message is (i.e. INFO/WARN/ERROR/FATAL)
Message is the actual message

void Logger::Print(const std::string& msg, LogType log) {
	assert(log >= 0 && log <= 4);
	const static enum Color ConsoleColor[4] = {
		Color::WHITE,
		Color::YELLOW,
		Color::RED,
		Color::RED
	};
	if (ConsoleDebugSettings.ColoredText) {
		Buffer << (char)ConsoleColor[log];
	}
	Buffer
		<< GetCurrentTimeString()
		<< GetThreadLoggingName(log)
		<< msg
		<< std::endl;
	OnPrint();
}

// https://stackoverflow.com/questions/1567082/how-do-i-iterate-over-cin-line-by-line-in-c/1567703#1567703
class ColoredTextLine {
public:
	friend std::istream& operator>>(std::istream& is, ColoredTextLine& l) {
		std::getline(is, l.Data);
		return is;
	}
	operator std::string() const {
		return Data;
	}
private:
	std::string Data;
};

void Logger::Flush(std::ostream& out) {
	if (ConsoleDebugSettings.ColoredText) {
		std::for_each(
			std::istream_iterator<ColoredTextLine>(std::istringstream(Buffer.str())),
			std::istream_iterator<ColoredTextLine>(),
			[](const std::string& line) -> void {
				// We will have to process each color indiviually 
				// That's why we can't use 1 IO operation for this
				Color LineColor = (Color)line[0];
				setcolor(LineColor);

				std::cout << line.substr(1, line.size()) << '\n';
			}
		);
	}
	else {
		// Print it to the output stream
		out << Buffer.str();
	}
	// Clear the buffer
	Buffer.str(std::string());
	Buffer.clear();
}

void Logger::SetConsoleDebugState(bool autoflush, bool color) {
	ConsoleDebugSettings.AutomaticFlush = autoflush;
	ConsoleDebugSettings.ColoredText = color;
}

void Logger::OnPrint(void) {
	if (ConsoleDebugSettings.AutomaticFlush) {
		Flush(std::cout);
	}
}

Logger& Logger::operator<<(const std::string& msg) {
	this->Print(msg);
	return *this;
}

Logger DefaultLogger; 
*/