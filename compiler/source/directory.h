/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <string>
#include <vector>

struct Directory
{
	static std::vector<std::filesystem::path> scan(const std::filesystem::path& path, const std::vector<std::string>& extensions, bool recursive);
};