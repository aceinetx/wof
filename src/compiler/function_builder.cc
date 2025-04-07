#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doFunction(SExprObject object) {
	Token &type = object.children[1].token;
	Token &name = object.children[2].token;
	SExprObject &args = object.children[3];
	SExprObject &body = object.children[4];

	std::map<std::string, Type *> args_map;

	if (type.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted a IDENTIFIER after fun", name.line);
		return false;
	}

	if (name.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted a IDENTIFIER after function name", name.line);
		return false;
	}

	for (SExprObject &arg : args.children) {
		if (arg.children.size() == 0) {
			ERROR("[{}] Argument is supposed to be wrapped in parens ()", name.line);
			return false;
		}

		if (arg.children.size() == 2) {
			Token arg_type = arg.children[0].token;
			Token arg_name = arg.children[1].token;

			if (arg_type.type != Token::IDENTIFIER) {
				ERROR("[{}] Argument type should be an identifier", arg_type.line);
				return false;
			}

			if (arg_name.type != Token::IDENTIFIER) {
				ERROR("[{}] Argument name should be an identifier", arg_name.line);
				return false;
			}

			args_map[arg_name.valueS] = getTypeFromName(arg_type.valueS);
		} else {
			ERROR("[{}] There should be only 2 child operands", name.line);
			return false;
		}
	}

	Type *retType = getTypeFromName(type.valueS);
	if (!retType) {
		return false;
	}

	WofFunction function;
	function.type = FunctionType::get(retType, false);
	function.function = Function::Create(function.type, Function::ExternalLinkage, name.valueS, fmodule);
	function.block = BasicBlock::Create(context, name.valueS + "b", function.function);

	currentFunction = name.valueS;

	functions[name.valueS] = function;

	builder.SetInsertPoint(function.block);

	// create arguments for future use
	auto fnArgs = function.function->arg_begin();
	Value *arg = fnArgs++;

	if (!doBody(body)) {
		return false;
	}

	return true;
}
