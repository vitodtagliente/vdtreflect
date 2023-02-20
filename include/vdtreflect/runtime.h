/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::map<std::string, int> enum_values_t;

template <typename T>
struct Enum
{
	static const char* name() { return ""; }
	static const enum_values_t& values() {
		static enum_values_t s_values;
		return s_values;
	}
};

class EnumFactory final
{
public:
	EnumFactory() = delete;

	template <typename T>
	friend struct RegisteredInEnumFactory;

	static std::string enumToString(const std::string& type, const int value)
	{
		for (const auto& pair : definition(type))
		{
			if (pair.second == value)
			{
				return pair.first;
			}
		}
		return "";
	}

	static bool stringToEnum(const std::string& type, const std::string& name, int& value)
	{
		const enum_values_t& def = definition(type);
		const auto& it = def.find(name);
		if (it != def.end())
		{
			return value = it->second, true;
		}
		return false;
	}

private:
	static std::map<std::string, enum_values_t>& collection()
	{
		static std::map<std::string, enum_values_t> s_getters;
		return s_getters;
	}

	static const enum_values_t& definition(const std::string& name)
	{
		static enum_values_t s_empty_definition;

		const auto& it = collection().find(name);
		if (it != collection().end())
		{
			return it->second;
		}
		return s_empty_definition;
	}

	static bool registerEnum(const std::string& name, const enum_values_t& values)
	{
		return collection().insert(std::make_pair(name, values)), true;
	}
};

template <typename T>
struct RegisteredInEnumFactory
{
	static bool value;
};

template <typename T>
bool RegisteredInEnumFactory<T>::value{ EnumFactory::registerEnum(Enum<T>::name(), Enum<T>::values()) };

template <class T>
std::string enumToString(const T t)
{
	const int value = static_cast<int>(t);
	for (const auto& pair : Enum<T>::values())
	{
		if (pair.second == value)
		{
			return pair.first;
		}
	}
	return "";
}

template <class T>
bool stringToEnum(const std::string& name, T& t)
{
	const enum_values_t& definition = Enum<T>::values();
	const auto& it = definition.find(name);
	if (it != definition.end())
	{
		return t = static_cast<T>(it->second), true;
	}
	return false;
}

typedef std::map<std::string, std::string> meta_t;
typedef unsigned long long member_address_t;

struct Property
{
	enum class Type
	{
		T_unknown,

		T_bool,
		T_char,
		T_double,
		T_float,
		T_int,
		T_void,

		T_container_map,
		T_container_string,
		T_container_vector,

		T_custom_enum,
		T_custom_type
	};

	enum class DecoratorType
	{
		D_normalized,
		D_pointer,
		D_reference,
		D_shared_ptr,
		D_unique_ptr,
		D_weak_ptr,
		D_unknown
	};

	struct TypeDescriptor
	{
		TypeDescriptor(const std::string& name, const Property::Type type, const Property::DecoratorType decoratorType, const std::vector<TypeDescriptor>& children)
			: name(name)
			, type(type)
			, decoratorType(decoratorType)
			, children(children)
		{

		}

		std::string name;
		Property::Type type;
		Property::DecoratorType decoratorType;
		std::vector<TypeDescriptor> children;
	};

	Property(const std::string& name, const TypeDescriptor& descriptor, const std::size_t size, const member_address_t address, const meta_t& meta)
		: name(name)
		, descriptor(descriptor)
		, size(size)
		, address(address)
		, meta(meta)
	{

	}

	const std::string name;
	TypeDescriptor descriptor;
	const member_address_t address{ 0 };
	const std::size_t size;
	const meta_t meta;

	template<typename T>
	T& value() const
	{
		return *reinterpret_cast<T*>(address);
	}
};
typedef std::map<std::string, Property> properties_t;

struct Type
{
	Type() = default;
	virtual ~Type() = default;

	virtual const std::string& getTypeName() const
	{
		static std::string s_name("Type");
		return s_name;
	}

	virtual const meta_t& getTypeMeta() const
	{
		static meta_t s_meta;
		return s_meta;
	}

	virtual const properties_t getTypeProperties() const
	{
		return {};
	}

	virtual std::size_t getTypeSize() const
	{
		return sizeof(Type);
	}
};
typedef std::function<Type* ()> factory_constructor_t;
typedef std::function<Type* ()> type_constructor_t;
typedef std::function<const meta_t& ()> factory_meta_t;

struct TypeDefinition
{
	TypeDefinition(const type_constructor_t& constructor, const std::string& name, const meta_t& meta, const std::size_t size)
		: constructor(constructor)
		, name(name)
		, meta(meta)
		, size(size)
	{}

	type_constructor_t constructor;
	std::string name;
	meta_t meta;
	std::size_t size;
};

class TypeFactory final
{
public:
	TypeFactory() = delete;

	static Type* instantiate(const std::string& name)
	{
		const auto& it = collection().find(name);
		if (it != collection().end())
		{
			return it->second.constructor();
		}
		return nullptr;
	}

	template <typename T>
	static T* instantiate()
	{
		return reinterpret_cast<T*>(instantiate(Type<T>::name()));
	}

	template <typename T>
	static T* instantiate(const std::string& name)
	{
		return reinterpret_cast<T*>(instantiate(name));
	}

	template <typename T>
	static T* instantiate(const TypeDefinition& type)
	{
		return reinterpret_cast<T*>(instantiate(type.name));
	}

	static std::vector<TypeDefinition> list()
	{
		std::vector<TypeDefinition> result;
		for (const auto& [typeName, type] : collection())
		{
			result.push_back(type);
		}
		return result;
	}

	static std::vector<TypeDefinition> list(const std::string& metaOption)
	{
		return TypeFactory::list(metaOption, "");
	}

	static std::vector<TypeDefinition> list(const std::string& metaOption, const std::string& metaValue)
	{
		std::vector<TypeDefinition> result;
		for (const auto& [typeName, type] : collection())
		{
			const auto& it = type.meta.find(metaOption);
			if (it != type.meta.end() && (metaValue.empty() || it->second == metaValue))
			{
				result.push_back(type);
			}
		}
		return result;
	}

	static bool registerType(const TypeDefinition& type)
	{
		return collection().insert(std::make_pair(type.name, type)), true;
	}

private:
	static std::map<std::string, TypeDefinition>& collection()
	{
		static std::map<std::string, TypeDefinition> s_types;
		return s_types;
	}
};

template <typename T>
struct RegisteredInTypeFactory
{
	static bool value;
};

template <typename T>
bool RegisteredInTypeFactory<T>::value{ TypeFactory::registerType(T::type()) };

#define ENUM(...)
#define CLASS(...)
#define PROPERTY(...)
#define FUNCTION(...)

#define GENERATED_BODY() \
	virtual const meta_t& getTypeMeta() const ; \
	virtual const std::string& getTypeName() const; \
	virtual const properties_t getTypeProperties() const; \
	virtual std::size_t getTypeSize() const; \
	static const meta_t& staticTypeMeta() ; \
	static const std::string& staticTypeName();