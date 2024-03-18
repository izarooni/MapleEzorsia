#pragma once
#include <iostream>
#include <string>
#include <ctime>

class Logger {
public:
	static void CreateConsole();
	static void Info(const char* message, ...);
	static void Warn(const char* message, ...);
};