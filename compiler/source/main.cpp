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
	std::vector<std::filesystem::path> includes;
	bool recursive = true;

	if (argc == 0)
	{
		std::cout << "Invalid arguments..." << std::endl;
		return COMPILER_ERROR;
	}

	bool includeFiles = false;
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
				includeFiles = false;
				recursive = false;
			}
			else if (argv[i] == std::string("-include"))
			{
				includeFiles = true;
			}
			else if (includeFiles)
			{
				includes.push_back(argv[i]);
			}
		}
	}

	std::cout << "input: " << in.string() << std::endl;
	std::cout << "recursion: " << (recursive ? "true" : "false") << std::endl;
	std::cout << "includes: " << std::endl;
	for (const std::filesystem::path& file : includes)
	{
		std::cout << "- "  << file.string() << std::endl;
	}
	std::cout << std::endl;

	if (!std::filesystem::exists(in))
	{
		std::cout << "Invalid input file" << std::endl;
		return COMPILER_ERROR;
	}

	FileCollection fileCollection;
	TypeCollection typeCollection;
	SymbolTable symbolTable;

	// includes
	for (const std::filesystem::path& file : includes)
	{
		if (std::filesystem::is_directory(file))
		{
			for (const std::filesystem::path& _file : Directory::scan(file, { ".h" }, recursive))
			{
				SymbolList symbolList;
				if (!Parser::parse(typeCollection, symbolTable, symbolList, _file))
				{
					std::cout << "Failed to parse the input file " << _file << std::endl;
					return COMPILER_ERROR;
				}
			}
		}
		else
		{
			SymbolList symbolList;
			if (!Parser::parse(typeCollection, symbolTable, symbolList, file))
			{
				std::cout << "Failed to parse the input file" << std::endl;
				return COMPILER_ERROR;
			}
		}
	}

	if (std::filesystem::is_directory(in))
	{
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
	}
	else
	{
		SymbolList symbolList;
		if (!Parser::parse(typeCollection, symbolTable, symbolList, in))
		{
			std::cout << "Failed to parse the input file" << std::endl;
			return COMPILER_ERROR;
		}

		fileCollection.insert(std::make_pair(in, symbolList));
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

	std::cout << "Compilation succeeded" << std::endl;
	return COMPILER_SUCCESS;
}