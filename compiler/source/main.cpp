#include <filesystem>
#include <iostream>

#include "directory.h"
#include "encoder.h"
#include "file_collection.h"
#include "parser.h"
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

	if (std::filesystem::is_directory(in))
	{
		FileCollection fileCollection;
		TypeCollection typeCollection;
		SymbolTable symbolTable;
		for (const std::filesystem::path& file : Directory::scan(in, { ".h" }, recursive))
		{
			SymbolList symbolList;
			if (!Parser::parse(typeCollection, symbolTable, symbolList, file))
			{
				std::cout << "Failed to parse the input file " << file << std::endl;
				return COMPILER_ERROR;
			}
			if (symbolList.empty()) continue;

			fileCollection.insert(std::make_pair(file, symbolList));
		}

		if (fileCollection.empty())
		{
			// Nothing to generate
			return COMPILER_SUCCESS;
		}

		for (auto& [file, symbols] : fileCollection)
		{
			if (!Encoder::encode(symbols, typeCollection, symbolTable, file.parent_path(), file.filename().string()))
			{
				std::cout << "Failed to encode the generated code for the file " << file.filename().string() << std::endl;
				return COMPILER_ERROR;
			}
		}
	}
	else
	{
		TypeCollection typeCollection;
		SymbolTable symbolTable;
		SymbolList symbolList;
		if (!Parser::parse(typeCollection, symbolTable, symbolList, in))
		{
			std::cout << "Failed to parse the input file" << std::endl;
			return COMPILER_ERROR;
		}

		if (symbolList.empty())
		{
			// Nothing to generate
			return COMPILER_SUCCESS;
		}

		if (!Encoder::encode(symbolList, typeCollection, symbolTable, in.parent_path(), in.filename().string()))
		{
			std::cout << "Failed to encode the generated code for the file " << in.filename().string() << std::endl;
			return COMPILER_ERROR;
		}
	}

	std::cout << "Compilation succeeded" << std::endl;
	return COMPILER_SUCCESS;
}