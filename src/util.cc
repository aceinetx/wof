#include <cassert>
#include <log.hpp>
#include <pch.hpp>
#include <util.hpp>

char* argsShift(int* argc, char*** argv) {
	assert(*argc > 0 && "argc <= 0");
	--(*argc);
	return *(*argv)++;
}

std::string readFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		ERROR("could not open {}", filename);
		exit(1);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}
