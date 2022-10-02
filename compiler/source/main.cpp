#include <filesystem>
#include <iostream>

#include "encoder.h"
#include "parser.h"
#include "string_util.h"

#define COMPILER_SUCCESS 1;
#define COMPILER_ERROR 0;

int main(int argc, char** argv)
{
	// input and output files
	std::string in; // = "C:/Users/vitod/Desktop/vdtproto/test/test1.h";

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
	}

	if (!std::filesystem::exists(in))
	{
		std::cout << "Invalid input file" << std::endl;
		return COMPILER_ERROR;
	}

	TypeCollection collection;

	Parser parser;
	if (!parser.parse(collection, in))
	{
		std::cout << "Failed to parse the input file" << std::endl;
		return COMPILER_ERROR;
	}

	Encoder encoder;
	if (!encoder.encode(collection, in))
	{
		std::cout << "Failed to encode the generated code" << std::endl;
		return COMPILER_ERROR;
	}

	return COMPILER_SUCCESS;
}