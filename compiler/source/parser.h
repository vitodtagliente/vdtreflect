/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <map>

#include "type.h"

class Parser
{
public:
	Parser() = default;

	bool parse(TypeCollection& collection, const std::filesystem::path& file);

private:
	bool parseClass(TypeCollection& collection, const std::vector<std::string>& tokens, size_t& index);
	bool parseEnum(TypeCollection& collection, const std::vector<std::string>& tokens, size_t& index);
	bool parseMeta(const std::vector<std::string>& tokens, size_t index, meta_t& meta);
};