#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doFunction(SExprObject object) {
	Token &type = object.children[1].token;
	Token &name = object.children[2].token;
	SExprObject &args = object.children[3];
	SExprObject &body = object.children[4];

	std::vector<Type *> argTypes;
	std::vector<std::string> argNames;

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
			Token argType = arg.children[0].token;
			Token argName = arg.children[1].token;

			if (argType.type != Token::IDENTIFIER) {
				ERROR("[{}] Argument type should be an identifier", argType.line);
				return false;
			}

			if (argName.type != Token::IDENTIFIER) {
				ERROR("[{}] Argument name should be an identifier", argName.line);
				return false;
			}

			argTypes.push_back(getTypeFromName(argType.valueS));
			argNames.push_back(argName.valueS);
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
	function.type = FunctionType::get(retType, argTypes, false);
	function.function = Function::Create(function.type, Function::ExternalLinkage, name.valueS, fmodule);
	function.block = BasicBlock::Create(context, name.valueS + "b", function.function);

	currentFunction = name.valueS;

	functions[name.valueS] = function;

	builder.SetInsertPoint(function.block);

	// create arguments for future use
	auto fnArgs = function.function->arg_begin();
	Value *arg = fnArgs++;
	for (int i = 0; i < argNames.size(); i++) {
		WofVariable variable;
		variable.name = argNames[i];
		variable.value = arg;

		functions[currentFunction].variables[variable.name] = variable;
		functions[currentFunction].function->getArg(i)->setName(variable.name);

		arg = fnArgs++;
	}

	if (!doBody(body)) {
		return false;
	}

	return true;
}
