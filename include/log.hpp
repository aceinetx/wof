#pragma once
#include <pch.hpp>

#define INFO(...) Log::getInstance().info(std::format(__VA_ARGS__))
#define WARN(...) Log::getInstance().warn(std::format(__VA_ARGS__))
#define ERROR(...) Log::getInstance().error(std::format(__VA_ARGS__))

class Log {
public:
	static Log& getInstance();

public:
	void info(std::string s);
	void warn(std::string s);
	void error(std::string s);
};
