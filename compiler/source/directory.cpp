#include "directory.h"

std::vector<std::filesystem::path> Directory::scan(const std::filesystem::path& path, const std::vector<std::string>& extensions, const bool recursive)
{
	std::vector<std::filesystem::path> result;
	if (std::filesystem::is_directory(path) == false) return result;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		const std::filesystem::path& file = entry.path();
		if (std::filesystem::is_directory(file) && recursive)
		{
			const auto& childFiles = scan(file, extensions, recursive);
			result.insert(result.end(), childFiles.begin(), childFiles.end());
		}
		else
		{
			const std::string extension = file.extension().string();
			if (std::find(extensions.begin(), extensions.end(), extension) != extensions.end())
			{
				result.push_back(file);
			}
		}
	}
	return result;
}
