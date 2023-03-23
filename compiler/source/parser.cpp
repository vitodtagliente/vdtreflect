#include "parser.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "string_util.h"

bool Parser::parse(TypeCollection& collection, SymbolTable& symbolTable, SymbolList& symbolList, const std::filesystem::path& file)
{
	static const auto read = [](const std::filesystem::path& filename) -> std::string
	{
		// std::ostringstream buf;
		// std::ifstream input(filename.c_str());
		// buf << input.rdbuf();
		// return buf.str();
		std::ostringstream buf;
		std::ifstream input(filename.c_str());
		if (input.is_open())
		{
			std::string line;
			while (std::getline(input, line))
			{
				if (line.empty()) continue;
				if (line.length() >= 2)
				{
					// skip comments
					if (line[0] == '/' && line[1] == '/') continue;
				}

				buf << line << '\n';
			}
			input.close();
		}
		return buf.str();
	};

	std::string content = read(file);
	if (content.empty()) return true;
	if (!StringUtil::contains(content, "ENUM", StringUtil::CompareMode::CaseSensitive) 
		&& !StringUtil::contains(content, "CLASS", StringUtil::CompareMode::CaseSensitive)) return true;

	// sanitize the content
	content = StringUtil::replace(content, "\n", " ");
	content = StringUtil::replace(content, "\t", " ");
	content = StringUtil::replace(content, "(", " ( ");
	content = StringUtil::replace(content, ")", " ) ");
	content = StringUtil::replace(content, "{", " { ");
	content = StringUtil::replace(content, "}", " } ");
	content = StringUtil::replace(content, ";", " ; ");
	content = StringUtil::replace(content, ",", " , ");
	content = StringUtil::replace(content, "  ", " ");
	// tokenize
	std::vector<std::string> tokens = StringUtil::split(content, ' ');
	tokens.erase(
		std::remove_if(
			tokens.begin(),
			tokens.end(),
			[](const std::string& token) -> bool { return token.empty(); }
		),
		tokens.end()
	);
	if (tokens.empty()) return true;

	size_t index = 0;
	while (index < tokens.size())
	{
		const std::string& keyword = tokens[index];
		if (keyword == "ENUM")
		{
			if (!parseEnum(collection, symbolTable, symbolList, tokens, index))
			{
				return false;
			}
		}
		else if (keyword == "CLASS")
		{
			if (!parseClass(collection, symbolTable, symbolList, tokens, index))
			{
				return false;
			}
		}
		else if (keyword == "NATIVE_CLASS")
		{
			if (!parseNativeClass(collection, symbolTable, symbolList, tokens, index))
			{
				return false;
			}
		}
		++index;
	}

	return true;
}

bool Parser::parseClass(TypeCollection& collection, SymbolTable& symbolTable, SymbolList& symbolList, const std::vector<std::string>& tokens, size_t& index)
{
	bool isStruct = false;

	const size_t startingIndex = index;
	TypeClass* element = nullptr;
	while (index + 1 < tokens.size())
	{
		if (tokens[index] == "class" || tokens[index] == "struct")
		{
			isStruct = tokens[index] == "struct";
			element = collection.addClass(tokens[(++index)++]);
			element->isStruct = isStruct;
			break;
		}
		++index;
	}

	if (element == nullptr) return false;
	symbolTable.insert(std::make_pair(element->name, SymbolType::S_class));
	symbolList.push_back(element->name);

	bool foundParent = false;
	while (index < tokens.size())
	{
		const std::string& token = tokens[index++];
		if (token == "{")
		{
			break;
		}

		if (foundParent) continue;

		if (token == ":"
			|| token == "public"
			|| token == "private"
			|| token == "protected"
			|| token == "final"
			|| token == ","
			) continue;

		if (element->parent != "IType")
		{
			element->parent = token;
		}
	}

	int openGraphs = 0;
	while (index < tokens.size())
	{
		std::string token = tokens[index++];
		if (token == "}" && openGraphs == 0 && element != nullptr)
		{
			parseMeta(tokens, startingIndex, element->meta);
			return true;
		}

		if (token == "{")
		{
			++openGraphs;
		}
		else if (token == "}")
		{
			if (--openGraphs < 0) return false;
		}
		else if (token == "PROPERTY")
		{
			const std::size_t startingPropertyIndex = index - 1;
			while (index + 1 < tokens.size())
			{
				if (tokens[++index] == ")") break;
			}

			if (index + 2 >= tokens.size()) return false;

			std::string propertyType = tokens[++index];
			if (propertyType.find('<') != std::string::npos)
			{
				while(propertyType.find('>') == std::string::npos && index + 1 < tokens.size())
				{
					propertyType += tokens[++index];
					if (propertyType.back() == ',')
					{
						propertyType += " ";
					}
				}
				if (propertyType.find('>') == std::string::npos) return false;
			}
			std::string propertyName = tokens[++index];
			meta_t propertyMeta;
			parseMeta(tokens, startingPropertyIndex, propertyMeta);
			element->addProperty(propertyName, propertyType, propertyMeta);
		}
	}

	return false;
}

bool Parser::parseEnum(TypeCollection& collection, SymbolTable& symbolTable, SymbolList& symbolList, const std::vector<std::string>& tokens, size_t& index)
{
	const size_t startingIndex = index;
	TypeEnum* element = nullptr;
	while (index + 1 < tokens.size())
	{
		if (tokens[index] == "class")
		{
			element = collection.addEnum(tokens[++index]);
			break;
		}
		++index;
	}

	if (element == nullptr) return false;
	symbolTable.insert(std::make_pair(element->name, SymbolType::S_enum));
	symbolList.push_back(element->name);

	while (index + 1 < tokens.size())
	{
		if (tokens[++index] == "{") break;
	}

	bool lookingForNextOptionName = true;
	while (index < tokens.size())
	{
		std::string token = tokens[++index];
		if (token == "}")
		{
			parseMeta(tokens, startingIndex, element->meta);
			return true;
		}

		if (token == ",")
		{
			lookingForNextOptionName = true;
			continue;
		}

		if (lookingForNextOptionName)
		{
			element->addOption(token);
			lookingForNextOptionName = false;
		}
	}
	return false;
}

bool Parser::parseNativeClass(TypeCollection& collection, SymbolTable& symbolTable, SymbolList& symbolList, const std::vector<std::string>& tokens, size_t& index)
{
	bool isStruct = false;

	const size_t startingIndex = index;
	if (index + 3 >= tokens.size()) return false;
	if (tokens[index + 1] != "(" || tokens[index + 3] != ")") return false;
	const std::string nativeTypeName = tokens[index + 2];

	index += 4;

	TypeClass* element = nullptr;
	while (index + 1 < tokens.size())
	{
		if (tokens[index] == "class" || tokens[index] == "struct")
		{
			isStruct = tokens[index] == "struct";
			element = collection.addClass(nativeTypeName);
			element->isStruct = isStruct;
			break;
		}
		++index;
	}

	if (element == nullptr) return false;
	symbolTable.insert(std::make_pair(element->name, SymbolType::S_nativeClass));
	symbolList.push_back(element->name);

	int openGraphs = 0;
	while (index < tokens.size())
	{
		std::string token = tokens[index++];
		if (token == ";" && openGraphs == 0 && element != nullptr)
		{
			return true;
		}

		if (token == "{")
		{
			++openGraphs;
		}
		else if (token == "}")
		{
			if (--openGraphs < 0) return false;
		}
		else if (token == "PROPERTY")
		{
			const std::size_t startingPropertyIndex = index - 1;
			while (index + 1 < tokens.size())
			{
				if (tokens[++index] == ")") break;
			}

			if (index + 2 >= tokens.size()) return false;

			std::string propertyType = tokens[++index];
			if (propertyType.find('<') != std::string::npos)
			{
				while (propertyType.find('>') == std::string::npos && index + 1 < tokens.size())
				{
					propertyType += tokens[++index];
					if (propertyType.back() == ',')
					{
						propertyType += " ";
					}
				}
				if (propertyType.find('>') == std::string::npos) return false;
			}
			std::string propertyName = tokens[++index];
			meta_t propertyMeta;
			parseMeta(tokens, startingPropertyIndex, propertyMeta);
			element->addProperty(propertyName, propertyType, propertyMeta);
		}
	}

	return false;
}

bool Parser::parseMeta(const std::vector<std::string>& tokens, size_t index, meta_t& meta)
{
	while (index + 1 < tokens.size())
	{
		if (tokens[++index] == "(") break;
	}
	++index;

	std::string temp;
	while (index < tokens.size())
	{
		const std::string& token = tokens[index++];
		if (token == ")")
		{
			break;
		}
		temp += " " + token;
	}

	temp = StringUtil::trim(temp);

	if (temp.empty()) return true;

	for (const std::string& token : StringUtil::split(temp, ','))
	{
		if (StringUtil::contains(token, "="))
		{
			const auto pieces = StringUtil::split(token, '=');
			meta.insert(std::make_pair(StringUtil::trim(pieces[0]), StringUtil::trim(pieces[1])));
		}
		else
		{
			meta.insert(std::make_pair(token, ""));
		}
	}

	return true;
}
