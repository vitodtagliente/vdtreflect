/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, std::string> meta_t;

enum class NativeType
{
	T_unknown,

	T_bool,
	T_char,
	T_double,
	T_enum,
	T_float,
	T_int,
	T_string,
	T_template,
	T_type,
	T_void
};

enum class DecoratorType
{
	D_raw,
	D_pointer,
	D_reference
};

class Type
{
public:
	Type() = default;
	virtual ~Type() = default;

	std::string name;
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

	std::string parent;
	std::vector<Property> properties;
	meta_t meta;
	bool isStruct{ false };

	TypeClass* const addProperty(const std::string& memberName, const std::string& type, const meta_t& meta);
};

class TypeEnum : public Type
{
public:
	TypeEnum() = default;

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
	TypeClass* const findClass(const std::string& name) const;
	TypeEnum* const addEnum(const std::string& name);
	TypeEnum* const findEnum(const std::string& name) const;

	bool empty() const;

private:
	std::vector<Type*> m_entities;
};