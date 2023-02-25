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

	static bool insert(const std::string& name, const enum_values_t& values)
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
bool RegisteredInEnumFactory<T>::value{ EnumFactory::insert(Enum<T>::name(), Enum<T>::values()) };

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

typedef std::map<std::string, std::string> type_meta_t;
typedef unsigned long long type_member_address_t;

struct NativeType
{
	enum class Type
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

	std::string name;
	std::vector<NativeType> children;
	DecoratorType decorator;
	std::size_t size;
	Type type;
};

struct Property
{
	std::size_t offset;
	const type_meta_t meta;
	const std::string name;
	const NativeType type;

	template<typename T, typename O>
	T& value(O* const object) const
	{
		return *reinterpret_cast<T*>(reinterpret_cast<type_member_address_t>(object) + offset);
	}
};
typedef std::map<std::string, Property> type_properties_t;

struct IType
{
	virtual const char* const type_name() const = 0;
	virtual const type_meta_t& type_meta() const = 0;
	virtual const type_properties_t& type_properties() const = 0;
};

template <typename T>
struct Type
{
	Type() = delete;

	static const type_meta_t& meta() {
		static type_meta_t s_meta;
		return s_meta;
	}
	static const char* const name() { return ""; }
	static std::size_t size() { return sizeof(T); }
};
typedef std::function<IType* ()> type_constructor_t;

class TypeFactory final
{
public:
	TypeFactory() = delete;

	template <typename T>
	friend struct RegisteredInTypeFactory;

	static IType* const instantiate(const std::string& name)
	{
		const auto& dictionary = collection();
		const auto& it = dictionary.find(name);
		if (it != dictionary.end())
		{
			const type_constructor_t& constructor = std::get<1>(it->second);
			IType* const type = constructor();
			return type;
		}
		return nullptr;
	}

	template <typename T = IType>
	static T* const instantiate()
	{
		return reinterpret_cast<T*>(instantiate(Type<T>::name()));
	}
	
	template <typename T = IType>
	static T* const instantiate(const std::string& name)
	{
		return reinterpret_cast<T*>(instantiate(name));
	}

	static const std::map<std::string, std::tuple<type_meta_t, type_constructor_t>>& list()
	{
		return collection();
	}

	static std::map<std::string, std::tuple<type_meta_t, type_constructor_t>> list(const std::string& metaOption)
	{
		return list(metaOption, "");
	}

	static std::map<std::string, std::tuple<type_meta_t, type_constructor_t>> list(const std::string& metaOption, const std::string& metaValue)
	{
		std::map<std::string, std::tuple<type_meta_t, type_constructor_t>> result;
		for (const auto& [name, tuple] : collection())
		{
			const type_meta_t& meta = std::get<0>(tuple);
			const auto& it = meta.find(metaOption);
			if (it != meta.end() && (metaValue.empty() || it->second == metaValue))
			{
				result.insert(std::make_pair(name, tuple));
			}
		}
		return result;
	}

private:
	static std::map<std::string, std::tuple<type_meta_t, type_constructor_t>>& collection()
	{
		static std::map<std::string, std::tuple<type_meta_t, type_constructor_t>> s_getters;
		return s_getters;
	}

	static const std::tuple<type_meta_t, type_constructor_t>& definition(const std::string& name)
	{
		static std::tuple<type_meta_t, type_constructor_t> s_empty_definition;

		const auto& it = collection().find(name);
		if (it != collection().end())
		{
			return it->second;
		}
		return s_empty_definition;
	}

	static bool insert(const std::string& name, const type_meta_t& meta, const type_constructor_t& constructor)
	{
		return collection().insert(std::make_pair(name, std::make_tuple(meta, constructor))), true;
	}
};

template <typename T>
struct RegisteredInTypeFactory
{
	static bool value;
};

template <typename T>
bool RegisteredInTypeFactory<T>::value{ TypeFactory::insert(Type<T>::name(), Type<T>::meta(), []() -> IType* { return new T(); }) };

#define ENUM(...)
#define CLASS(...)
#define PROPERTY(...)
#define FUNCTION(...)
#define GENERATED_BODY() \
	template <typename T> \
	friend struct Type; \
	virtual const type_meta_t& type_meta() const override; \
	virtual const char* const type_name() const override; \
	virtual const type_properties_t& type_properties() const override;