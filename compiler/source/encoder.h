/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <sstream>
#include <stdarg.h>
#include <string>
#include <vector>

#include "file_collection.h"
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

	std::string string(bool useEndline = true) const;

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
	static bool encode(const SymbolList& symbolList, const TypeCollection& collection, const SymbolTable& symbolTable, const std::filesystem::path& path, const std::string& filename);

private:
	static bool encode(EncodeBuffer& headerBuffer, EncodeBuffer& sourceBuffer, const SymbolTable& symbolTable, const TypeCollection& collection, TypeClass& type);
	static bool encode(EncodeBuffer& headerBuffer, EncodeBuffer& sourceBuffer, const SymbolTable& symbolTable, TypeEnum& type);

	static std::string encodePropertyReflection(const std::string& offset, const SymbolTable& symbolTable, const Property& property, const std::string& name);
	static std::string encodePropertyReflection(const std::string& offset, const SymbolTable& symbolTable, const std::string& type);

	static std::string encodePropertySerialization(const std::string& offset, const SymbolTable& symbolTable, const bool serialize, const Property& property);
	static std::string encodePropertySerialization(const std::string& offset, const SymbolTable& symbolTable, const bool serialize, const std::string& name, const std::string& type);

	static std::string encodePropertySerializationToJson(const std::string& offset, const SymbolTable& symbolTable, const bool serialize, const Property& property);
	static std::string encodePropertySerializationToJson(const std::string& offset, const SymbolTable& symbolTable, const bool serialize, const std::string& name, const std::string& type);
	
	static NativeType parseNativeType(const SymbolTable& symbolTable, const std::string& type);
	static DecoratorType parseDecoratorType(const SymbolTable& symbolTable, const std::string& type);
	static std::vector<std::string> extractTypenames(const std::string& token);
	static std::string sanitizeTemplate(const std::string& token);

	static std::string toString(const NativeType type);
	static std::string toString(const DecoratorType type);

	static bool isValidListType(const NativeType type);
	static bool isValidMapKeyType(const NativeType type);
	static bool isValidMapValueType(const NativeType type);
};