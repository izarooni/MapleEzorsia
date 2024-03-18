#include "stdafx.h"
#include "Logger.h"
#include <ctime>

void Logger::CreateConsole() {
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
}

void Logger::Info(const char* message, ...) {
	// Get the current timestamp
	std::time_t now = std::time(nullptr);
	struct tm timeInfo;
	localtime_s(&timeInfo, &now);

	// Format the timestamp
	char timestamp[20];
	std::strftime(timestamp, sizeof(timestamp), "[%m-%d %H:%M:%S] ", &timeInfo);

	// Write to console
	char buf[1024] = { 0 };
	va_list args;
	va_start(args, message);
	vsprintf_s(buf, message, args);
	std::cout << timestamp << "[INFO] " << buf << std::endl;
	va_end(args);
}

void Logger::Warn(const char* message, ...) {
	// Get the current timestamp
	std::time_t now = std::time(nullptr);
	struct tm timeInfo;
	localtime_s(&timeInfo, &now);

	// Format the timestamp
	char timestamp[20];
	std::strftime(timestamp, sizeof(timestamp), "[%m-%d %H:%M:%S] ", &timeInfo);

	// Write to console
	char buf[1024] = { 0 };
	va_list args;
	va_start(args, message);
	vsprintf_s(buf, message, args);
	std::cout << timestamp << "[WARN] " << buf << std::endl;
	va_end(args);
}
