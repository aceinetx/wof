#include <compiler/compiler.hpp>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TargetParser/Host.h>
#include <log.hpp>

using namespace llvm;

Compiler::Compiler(std::string moduleName, Lexer &lexer) : builder(context), fmodule(moduleName, context), sexpr(lexer) {
	InitializeNativeTarget();
	InitializeAllTargetInfos();
	InitializeAllTargets();
	InitializeAllTargetMCs();
	InitializeAllAsmParsers();
	InitializeAllAsmPrinters();

	addBasicTypes();
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
	types["ptr"] = PointerType::get(builder.getVoidTy(), 0);
}

Type *Compiler::getTypeFromName(std::string name) {
	if (!types.contains(name)) {
		ERROR("Invalid type: {}", name);
		return nullptr;
	}

	return types[name];
}

bool Compiler::compile() {
	SExprObject object = sexpr.next();
	while (object.token.type != Token::END) {
		if (object.children.size() == 5) {
			if (object.children[0].token.valueS == "fun") {
				if (!doFunction(object))
					return false;
			}
		}
		if (object.children.size() > 0) {
			if (object.children[0].token.valueS == "extern") {
				if (!doExtern(object))
					return false;
			}
		}
		object = sexpr.next();
	}

	verifyModule(fmodule);

	return true;
}

bool Compiler::emitObject(std::string output) {
	std::string targetTriple = sys::getDefaultTargetTriple();

	std::string error;
	auto target = TargetRegistry::lookupTarget(targetTriple, error);
	if (!target) {
		ERROR("target lookup error: {}", error);
		return false;
	}

	auto cpu = "generic";
	auto features = "";

	TargetOptions opt;
	auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, Reloc::PIC_);

	fmodule.setDataLayout(targetMachine->createDataLayout());
	fmodule.setTargetTriple(targetTriple);

	std::error_code EC;
	raw_fd_ostream dest(output, EC, sys::fs::OF_None);
	if (EC) {
		ERROR("could not open output file: {}", EC.message());
		return false;
	}

	legacy::PassManager pass;
	auto fileType = CodeGenFileType::ObjectFile;
	if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
		ERROR("TargetMachine can't emit a file of this type");
		return false;
	}

	pass.run(fmodule);
	dest.flush();

	return true;
}

bool Compiler::smartEmit(std::string output) {
	if (!output.ends_with(".ll") || output.ends_with(".o")) { // compile object
		bool status = emitObject(output);
		if (status) {
			INFO("emitted object code to {}", output);
		}
		return status;
	} else { // emit ir
		std::error_code EC;
		raw_fd_ostream dest(output, EC, sys::fs::OF_None);
		if (EC) {
			ERROR("could not open output file: {}", EC.message());
			return false;
		}

		fmodule.print(dest, nullptr);
		INFO("emitted ir code to {}", output);
	}
	return true;
}
