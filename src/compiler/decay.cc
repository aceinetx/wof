#include <compiler/compiler.hpp>
#include <log.hpp>

bool Compiler::decayStruct(WofStruct& strukt, WofVariable& struktVar) {
	for (int i = 0; i < strukt.fieldTypes.size(); i++) {
		std::string fieldName = struktVar.name + "_" + strukt.fieldNames[i];
		if (std::count(strukt.privateFields.begin(), strukt.privateFields.end(), strukt.fieldNames[i]) > 0) {
			continue;
		}

		WofVariable sv;
		sv.value = builder.CreateStructGEP(strukt.type, struktVar.value, i, fieldName);
		sv.name = fieldName;
		sv.type = strukt.fieldTypes[i];
		functions[currentFunction].variables[sv.name] = sv;
	}

	return true;
}
