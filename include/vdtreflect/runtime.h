/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <string>

template <typename T>
struct EnumType
{
	static const char* name() { return ""; }
	static const std::map<const char*, int> values() {
		static std::map<const char*, int> s_values;
		return s_values;
	}
};

template <class T>
std::string enumToString(const T t)
{
	const int value = static_cast<int>(t);
	for (const auto& pair : EnumType<T>::values())
	{
		if (pair.second == value)
		{
			return pair.first;
		}
	}
	return "";
}

template <class T>
bool stringToEnum(const char* name, T& t)
{
	for (const auto& pair : EnumType<T>::values())
	{
		if (std::strcmp(pair.first, name) == 0)
		{
			return t = static_cast<T>(pair.second), true;
		}
	}
	return false;
}

template <class T>
bool stringToEnum(const std::string& name, T& t)
{
	for (const auto& pair : EnumType<T>::values())
	{
		if (name == pair.first)
		{
			return t = static_cast<T>(pair.second), true;
		}
	}
	return false;
}

typedef unsigned long long member_address_t;

enum class NativeType
{
	NT_invalid,
	NT_bool,
	NT_char,
	NT_double,
	NT_enum,
	NT_float,
	NT_int,
	NT_string,
	NT_void
};

typedef std::map<std::string, std::string> meta_t;

struct Property
{
	Property(const std::string& name, const NativeType type, const std::string& typeStr, const std::size_t size, const member_address_t address, const meta_t& meta)
		: name(name)
		, type(type)
		, typeStr(typeStr)
		, size(size)
		, address(address)
		, meta(meta)
	{

	}

	const std::string name;
	const NativeType type{ NativeType::NT_invalid };
	const std::string typeStr;
	const std::size_t size;
	const member_address_t address{ 0 };
	const meta_t meta;

	template<typename T>
	T& value() const
	{
		return *reinterpret_cast<T*>(address);
	}
};
typedef std::map<std::string, Property> properties_t;

struct IType
{
	IType() = default;
	virtual ~IType() = default;

	virtual const char* getTypeName() const = 0;
	virtual const meta_t& getTypeMeta() const = 0;
	virtual const properties_t getTypeProperties() const = 0;
	virtual std::size_t getTypeSize() const = 0;
};
typedef std::function<IType* ()> factory_constructor_t;
typedef std::function<const meta_t& ()> factory_meta_t;

struct TypeFactoryRegister
{
	static std::map<std::string, factory_constructor_t>& constructors()
	{
		static std::map<std::string, factory_constructor_t> s_register;
		return s_register;
	}

	static std::map<std::string, factory_meta_t>& meta()
	{
		static std::map<std::string, factory_meta_t> s_register;
		return s_register;
	}

	static bool load(const std::string& name, const factory_meta_t& meta_handler, const factory_constructor_t& constructor_handler)
	{
		meta().insert(std::make_pair(name, meta_handler));
		constructors().insert(std::make_pair(name, constructor_handler));
		return true;
	}
};

struct TypeFactory final
{
	static IType* instantiate(const std::string& name)
	{
		const std::map<std::string, factory_constructor_t>& constructors = TypeFactoryRegister::constructors();
		const auto& it = constructors.find(name);
		if (it != constructors.end())
		{
			return it->second();
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

	static std::vector<std::string> list()
	{
		std::vector<std::string> result;
		for (const auto& [typeName, constructor] : TypeFactoryRegister::constructors())
		{
			result.push_back(typeName);
		}
		return result;
	}

	static std::vector<std::string> list(const std::string& metaOption)
	{
		return TypeFactory::list(metaOption, "");
	}

	static std::vector<std::string> list(const std::string& metaOption, const std::string& metaValue)
	{
		std::vector<std::string> result;
		const std::map<std::string, factory_meta_t>& typeMeta = TypeFactoryRegister::meta();
		for (const auto& [typeName, metaHandler] : typeMeta)
		{
			const meta_t& meta = metaHandler();
			const auto& it = meta.find(metaOption);
			if (it != meta.end() && (metaValue.empty() || it->second == metaValue))
			{
				result.push_back(typeName);
			}
		}
		return result;
	}
};

template <typename T>
struct RegisteredInTypeFactory
{
	static bool value;
};

template <typename T>
bool RegisteredInTypeFactory<T>::value{ TypeFactoryRegister::load(T::name(), []() -> const meta_t& { return T::meta(); }, []() -> IType* { return T::instantiate(); }) };

#define ENUM(...)
#define CLASS(...)
#define PROPERTY(...)
#define FUNCTION(...)

#define GENERATED_BODY() \
	virtual const char* getTypeName() const; \
	virtual const meta_t& getTypeMeta() const ; \
	virtual const properties_t getTypeProperties() const; \
	virtual std::size_t getTypeSize() const;