/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <map>
#include <string>

enum class SymbolType
{
	S_class,
	S_nativeClass,
	S_enum,
};

typedef std::map<std::string, SymbolType> SymbolTable;
typedef std::vector<std::string> SymbolList;
typedef std::map<std::filesystem::path, SymbolList> FileCollection;