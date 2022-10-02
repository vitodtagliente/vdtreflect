#include "parser.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "string_util.h"

bool Parser::parse(TypeCollection& collection, const std::filesystem::path& file)
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
			if (!parseEnum(collection, tokens, index))
			{
				return false;
			}
		}
		else if (keyword == "CLASS")
		{
			if (!parseClass(collection, tokens, index))
			{
				return false;
			}
		}
		++index;
	}

	return true;
}

bool Parser::parseClass(TypeCollection& collection, const std::vector<std::string>& tokens, size_t& index)
{
	const size_t startingIndex = index;
	TypeClass* element = nullptr;
	while (index + 1 < tokens.size())
	{
		if (tokens[index] == "class")
		{
			element = collection.addClass(tokens[(++index)++]);
			break;
		}
		++index;
	}

	if (element == nullptr) return false;

	bool foundClassOpening = false;
	while (index < tokens.size())
	{
		const std::string& token = tokens[index++];
		if (token == "{")
		{
			foundClassOpening = true;
			break;
		}

		if (token == ":"
			|| token == "public"
			|| token == "private"
			|| token == "protected"
			) continue;

		if (element->parent != "IType")
		{
			element->parent = token;
		}
	}

	if (!foundClassOpening) return false;

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

			const std::string propertyType = tokens[++index];
			std::string propertyName = tokens[++index];
			meta_t propertyMeta;
			parseMeta(tokens, startingPropertyIndex, propertyMeta);
			element->addProperty(propertyName, propertyType, propertyMeta);
		}
	}

	return false;
}

bool Parser::parseEnum(TypeCollection& collection, const std::vector<std::string>& tokens, size_t& index)
{
	const size_t startingIndex = index;
	TypeEnum* element = nullptr;
	while (index + 1 < tokens.size())
	{
		if (tokens[index] == "class")
		{
			element = collection.addEnum(tokens[++index]);
			if (tokens[(++index)++] != "{") return false;
			break;
		}
		++index;
	}

	if (element == nullptr) return false;

	bool lookingForNextOptionName = true;
	while (index < tokens.size())
	{
		std::string token = tokens[index];
		if (token == "}")
		{
			parseMeta(tokens, startingIndex, element->meta);
			return true;
		}

		if (token == ",")
		{
			lookingForNextOptionName = true;
			++index;
			continue;
		}

		if (lookingForNextOptionName)
		{
			element->addOption(token);
			lookingForNextOptionName = false;
		}
		++index;
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
