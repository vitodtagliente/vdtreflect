/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <map>
#include <string>

enum class SymbolType
{
	S_class,
	S_enum,
};

typedef std::map<std::string, SymbolType> SymbolTable;