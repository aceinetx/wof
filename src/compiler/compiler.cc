#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

Compiler::Compiler(std::string moduleName, Lexer &lexer) : builder(context), fmodule(moduleName, context), sexpr(lexer) {
}

void Compiler::addBasicTypes() {
	types["u8"] = builder.getInt8Ty();
	types["u16"] = builder.getInt16Ty();
	types["u32"] = builder.getInt32Ty();
	types["u64"] = builder.getInt64Ty();

	types["i8"] = builder.getInt8Ty();
	types["i16"] = builder.getInt16Ty();
	types["i32"] = builder.getInt32Ty();
	types["i64"] = builder.getInt64Ty();

	types["f32"] = builder.getFloatTy();
	types["f64"] = builder.getDoubleTy();

	types["u0"] = builder.getVoidTy();
}

Type *Compiler::getTypeFromName(std::string name) {
	if (!types.count(name)) {
		ERROR("Invalid type: {}", name);
		return nullptr;
	}

	return types[name];
}

bool Compiler::compile() {
	SExprObject object = sexpr.next();
	while (object.token.type != Token::END) {
		if (object.children.size() == 4) {
			if (object.children[0].token.valueS == "fun") {
				if (!doFunction(object))
					return false;
			}
		}
		object = sexpr.next();
	}

	verifyModule(fmodule);

	std::error_code EC;
	raw_fd_ostream dest(STDOUT_FILENO, false);
	fmodule.print(dest, nullptr);

	return true;
}
