/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <map>

#include "symbol_table.h"
#include "type.h"

class Parser
{
public:
	static bool parse(TypeCollection& collection, SymbolTable& symbolTable, const std::filesystem::path& file);

private:
	static bool parseClass(TypeCollection& collection, SymbolTable& symbolTable, const std::vector<std::string>& tokens, size_t& index);
	static bool parseEnum(TypeCollection& collection, SymbolTable& symbolTable, const std::vector<std::string>& tokens, size_t& index);
	static bool parseMeta(const std::vector<std::string>& tokens, size_t index, meta_t& meta);
};