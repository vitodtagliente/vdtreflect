#include <filesystem>
#include <iostream>

#include "directory.h"
#include "encoder.h"
#include "parser.h"
#include "symbol_table.h"
#include "string_util.h"

#define COMPILER_SUCCESS 0;
#define COMPILER_ERROR 1;

int main(int argc, char** argv)
{
	// input and output files
	std::filesystem::path in; // = "C:/Users/vitod/Desktop/vdtproto/test/test1.h";
	bool recursive = true;

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
		else
		{
			if (argv[i] == std::string("-no-recursion"))
			{
				recursive = false;
			}
		}
	}

	if (!std::filesystem::exists(in))
	{
		std::cout << "Invalid input file" << std::endl;
		return COMPILER_ERROR;
	}

	SymbolTable symbolTable;
	if (std::filesystem::is_directory(in))
	{
		std::map<std::filesystem::path, TypeCollection> collections;
		for (const std::filesystem::path& file : Directory::scan(in, { ".h" }, recursive))
		{
			TypeCollection collection;
			if (!Parser::parse(collection, symbolTable, file))
			{
				std::cout << "Failed to parse the input file" << std::endl;
				return COMPILER_ERROR;
			}
			if (collection.empty()) continue;

			collections.insert(std::make_pair(file, collection));
		}

		if (collections.empty())
		{
			// Nothing to generate
			return COMPILER_SUCCESS;
		}

		for (auto& [file, collection] : collections)
		{
			if (!Encoder::encode(collection, symbolTable, file.parent_path(), file.filename().string()))
			{
				std::cout << "Failed to encode the generated code for the file " << in.filename().string() << std::endl;
				return COMPILER_ERROR;
			}
		}
	}
	else
	{
		TypeCollection collection;
		if (!Parser::parse(collection, symbolTable, in))
		{
			std::cout << "Failed to parse the input file" << std::endl;
			return COMPILER_ERROR;
		}

		if (collection.empty())
		{
			// Nothing to generate
			return COMPILER_SUCCESS;
		}

		if (!Encoder::encode(collection, symbolTable, in.parent_path(), in.filename().string()))
		{
			std::cout << "Failed to encode the generated code for the file " << in.filename().string() << std::endl;
			return COMPILER_ERROR;
		}
	}

	return COMPILER_SUCCESS;
}