/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, std::string> meta_t;

class Type
{
public:
	Type() = default;
	virtual ~Type() = default;
};

struct Property
{
	std::string name;
	std::string type;
	meta_t meta;
};

class TypeClass : public Type
{
public:
	TypeClass() = default;

	std::string name;
	std::string parent;
	std::vector<Property> properties;
	meta_t meta;

	TypeClass* const addProperty(const std::string& memberName, const std::string& type, const meta_t& meta);
};

class TypeEnum : public Type
{
public:
	TypeEnum() = default;

	std::string name;
	std::vector<std::string> options;
	meta_t meta;

	TypeEnum* const addOption(const std::string& optionName);
};

class TypeCollection
{
public:
	friend class Encoder;

	TypeCollection() = default;
	~TypeCollection();

	TypeClass* const addClass(const std::string& name);
	TypeEnum* const addEnum(const std::string& name);

	bool empty() const;

private:
	std::vector<Type*> m_entities;
};