#include <compiler/compiler.hpp>
#include <lexer.hpp>
#include <log.hpp>
#include <pch.hpp>
#include <sexpr.hpp>
#include <util.hpp>

int main(int argc, char** argv) {
	unsigned int i;
	std::vector<std::string> sources;
	std::string code;
	std::string output = "a.out";
	std::string moduleName = "wof";

	argsShift(&argc, &argv);
	for (i = 0; argc; ++i) {
		std::string arg = argsShift(&argc, &argv);

		if (arg == "--help" || arg == "-h") {
			std::cout << R"(compiler for wof (aceinetx 2022-2025)
usage: wofc [OPTION]... [INPUT]
options:
  --help (-h)        print help
  --out (-o)         set output filename
                     (note: if doesn't end with .ll (or ends with .o), it will emit
                      an object file, if does end with .ll, then IR code)
  --mod (-m)         set module name
)";
			return 0;
		} else if (arg == "--out" || arg == "-o") {
			output = argsShift(&argc, &argv);
		} else if (arg == "--mod" || arg == "-m") {
			moduleName = "wof_" + std::string(argsShift(&argc, &argv));
		} else {
			// assume it's the source
			sources.push_back(arg);
		}
	}

	if (moduleName == "wof") {
		moduleName += "_" + output;
		std::replace(moduleName.begin(), moduleName.end(), '.', '_');
	}

	if (sources.empty()) {
		ERROR("no sources provided");
		return 1;
	}

	for (std::string& source : sources) {
		code += readFile(source) + "\n\n";
	}

	Lexer lexer(code);

	Compiler compiler(moduleName, lexer);

	bool status;

	status = compiler.compile();
	if (!status)
		return 1;

	status = compiler.smartEmit(output);
	if (!status)
		return 1;

	return 0;
}
