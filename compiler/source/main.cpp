#include <filesystem>
#include <iostream>

#include "encoder.h"
#include "parser.h"
#include "string_util.h"

#define COMPILER_SUCCESS 0;
#define COMPILER_ERROR 1;

int main(int argc, char** argv)
{
	// input and output files
	std::filesystem::path in; // = "C:/Users/vitod/Desktop/vdtproto/test/test1.h";
	std::filesystem::path path;

	if (argc == 0)
	{
		std::cout << "Invalid arguments..." << std::endl;
		return COMPILER_ERROR;
	}

	for (int i = 1; i < argc; ++i)
	{
		if (i == 1)
		{
			in = argv[i];
		}
		else if (i == 2)
		{
			path = argv[i];
		}
	}

	if (!std::filesystem::exists(in))
	{
		std::cout << "Invalid input file" << std::endl;
		return COMPILER_ERROR;
	}

	if (path.empty())
	{
		path = std::filesystem::path(in).parent_path();
	}

	TypeCollection collection;

	Parser parser;
	if (!parser.parse(collection, in))
	{
		std::cout << "Failed to parse the input file" << std::endl;
		return COMPILER_ERROR;
	}

	if (collection.empty())
	{
		// Nothing to generate
		return COMPILER_SUCCESS;
	}

	Encoder encoder;
	if (!encoder.encode(collection, path, in.filename().string()))
	{
		std::cout << "Failed to encode the generated code" << std::endl;
		return COMPILER_ERROR;
	}

	return COMPILER_SUCCESS;
}