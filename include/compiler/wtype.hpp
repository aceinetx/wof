#pragma once

#include <pch.hpp>

/*
	Advanced LLVM type structure (Wide type).
	You can say this is kind of a workaround around opaque pointer types,
	and this is exactly what it is
*/
typedef struct wtype {
	llvm::Type *type;

	llvm::Type *pointee = nullptr;
	unsigned int pointCount = 0;

	wtype();
	wtype(llvm::Type *type = nullptr, llvm::Type *pointee = nullptr, unsigned int pointCount = 0);

	operator llvm::Type *() const;
} wtype;