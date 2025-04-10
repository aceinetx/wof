#include <compiler/wtype.hpp>

wtype::wtype() {
	type = nullptr;
}

wtype::wtype(llvm::Type *type, llvm::Type *pointee, unsigned int pointCount) {
	this->type = type;
	this->pointee = pointee;
	this->pointCount = pointCount;
}

wtype::operator llvm::Type *() const {
	return this->type;
}