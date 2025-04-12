#include <log.hpp>

Log& Log::getInstance() {
	static Log instance;
	return instance;
}

void Log::info(std::string s) {
	std::cout << "[wof] " << s << "\n";
}

void Log::warn(std::string s) {
	std::cout << "[\x1b[1;33mwof\x1b[0m] " << s << "\n";
}

void Log::error(std::string s) {
	std::cout << "[\x1b[1;31mwof\x1b[0m] " << s << "\n";
}
