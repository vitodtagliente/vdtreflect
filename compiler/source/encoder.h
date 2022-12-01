/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <sstream>
#include <stdarg.h>
#include <string>
#include <vector>

#include "symbol_table.h"
#include "type.h"

struct EncodeBuffer
{
public:
	friend class Encoder;

	EncodeBuffer() = default;

	void push(const std::string& token);
	template<typename... Ts>
	void push(Ts const&... ts)
	{
		const std::string str = string_format(ts...);
		push(str);
	}

	void push_line(const std::string& line);
	template<typename... Ts>
	void push_line(Ts const&... ts)
	{
		const std::string str = string_format(ts...);
		push_line(str);
	}

	std::string string() const;

private:

	template<typename... Ts>
	std::string string_format(Ts const&... ts) {
		std::stringstream s;
		int dummy[] = { 0, ((s << ts), 0)... };
		static_cast<void>(dummy); // Avoid warning for unused variable
		return s.str();
	}

	std::vector<std::string> m_lines;
};

class Encoder
{
public:
	static bool encode(const TypeCollection& collection, const SymbolTable& symbolTable, const std::filesystem::path& path, const std::string& filename);

private:
	static bool encode(EncodeBuffer& headerBuffer, EncodeBuffer& sourceBuffer, const SymbolTable& symbolTable, TypeClass& type);
	static bool encode(EncodeBuffer& headerBuffer, EncodeBuffer& sourceBuffer, const SymbolTable& symbolTable, TypeEnum& type);
	static std::string encode(const SymbolTable& symbolTable, const std::string& name, const std::string& offset, const Property& property);
	static std::string encode(const SymbolTable& symbolTable, const std::string& type);
	static std::string encodeToTypeEnum(const SymbolTable& symbolTable, const std::string& type);
	static std::string encodeToDecoratorTypeEnum(const SymbolTable& symbolTable, const std::string& type);
	static std::vector<std::string> extractTypenames(const std::string& token);
};