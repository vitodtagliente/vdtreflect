/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <sstream>
#include <stdarg.h>
#include <string>
#include <vector>

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
	Encoder() = default;

	bool encode(TypeCollection& collection, const std::filesystem::path& path, const std::string& filename);

private:
	bool encode(EncodeBuffer& headerBuffer, EncodeBuffer& sourceBuffer, TypeClass& type);
	bool encode(EncodeBuffer& headerBuffer, EncodeBuffer& sourceBuffer, TypeEnum& type);
};