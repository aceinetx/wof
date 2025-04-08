#include <cassert>
#include <pch.hpp>
#include <util.hpp>

char *argsShift(int *argc, char ***argv) {
	assert(*argc > 0 && "argc <= 0");
	--(*argc);
	return *(*argv)++;
}

std::string readFile(const std::string &filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Error opening file: " << filename << "\n";
		exit(1);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}
