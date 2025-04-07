#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

std::string getTypeName(llvm::Type *type) {
	std::string typeName;
	llvm::raw_string_ostream rso(typeName);
	type->print(rso);
	return rso.str();
}

Value *Compiler::castValue(Value *value, Type *targetType) {
	// Check if the value is already of the target type
	if (value->getType() == targetType) {
		return value; // No cast needed
	}

	// Handle pointer to integer casts and vice versa
	if (PointerType *ptrType = dyn_cast<PointerType>(value->getType())) {
		if (targetType->isIntegerTy()) {
			// Pointer to Integer cast
			return builder.CreatePtrToInt(value, targetType);
		}
	} else if (PointerType *targetPtrType = dyn_cast<PointerType>(targetType)) {
		if (value->getType()->isIntegerTy()) {
			// Integer to Pointer cast
			return builder.CreateIntToPtr(value, targetPtrType);
		}
	}

	// Perform the cast based on the types
	if (value->getType()->isIntegerTy() && targetType->isIntegerTy()) {
		// Integer to Integer cast
		return builder.CreateIntCast(value, targetType, true);
	} else if (value->getType()->isFloatingPointTy() && targetType->isFloatingPointTy()) {
		// Float to Float cast
		return builder.CreateFPCast(value, targetType);
	} else if (value->getType()->isIntegerTy() && targetType->isFloatingPointTy()) {
		// Integer to Float cast
		return builder.CreateSIToFP(value, targetType);
	} else if (value->getType()->isFloatingPointTy() && targetType->isIntegerTy()) {
		// Float to Integer cast
		return builder.CreateFPToSI(value, targetType);
	} else if (value->getType()->isIntegerTy() && targetType->isIntegerTy()) {
		// Handle cases where the bit sizes differ
		IntegerType *sourceType = dyn_cast<IntegerType>(value->getType());
		IntegerType *targetIntType = dyn_cast<IntegerType>(targetType);

		if (sourceType && targetIntType) {
			if (sourceType->getBitWidth() > targetIntType->getBitWidth()) {
				// Source type has more bits than target type, perform truncation
				return builder.CreateTrunc(value, targetType);
			} else if (sourceType->getBitWidth() < targetIntType->getBitWidth()) {
				// Source type has fewer bits than target type, perform extension
				return builder.CreateZExt(value, targetType); // Use CreateSExt for signed extension
			}
		}
	}

	// Handle other cases or throw an error
	ERROR("Unsupported cast from {} to {}", getTypeName(value->getType()), getTypeName(targetType));
	return nullptr;
}
