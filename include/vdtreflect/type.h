/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <functional>
#include <map>
#include <set>

namespace reflect
{
	typedef std::function<void* ()> factory_constructor_t;

	struct TypeFactoryImp
	{
		static std::map<std::string, std::set<std::string>>& categories();
		static std::map<std::string, factory_constructor_t>& data();

		static void hook(const std::string& name, const std::string& category, factory_constructor_t handler);
	};

	struct TypeDescriptor
	{
		TypeDescriptor(std::function<void(TypeDescriptor*)> init);

		template<typename T>
		static const TypeDescriptor& get()
		{
			return T::s_typeDescriptor;
		}

		std::string name;
		size_t size;
	};

	struct TypeFactory final
	{
		static void* instantiate(const std::string& name);

		template <typename T>
		static T* instantiate()
		{
			return reinterpret_cast<T*>(instantiate(TypeDescriptor::get<T>().name));
		}

		template <typename T>
		static T* instantiate(const std::string& name)
		{
			return reinterpret_cast<T*>(instantiate(name));
		}

		static std::set<std::string> list(const std::string& category);

		template <typename T>
		static void load()
		{
			TypeDescriptor::get<T>();
		}
	};

#define REFLECT() \
public: \
	friend struct TypeDescriptor; \
	virtual const TypeDescriptor& getTypeDescriptor() const; \
private: \
    static TypeDescriptor s_typeDescriptor; \
    static void registerTypeDescriptor(TypeDescriptor*); 

#define REFLECT_IMP_CATEGORY(T, C) \
    TypeDescriptor T::s_typeDescriptor{T::registerTypeDescriptor}; \
    \
    void T::registerTypeDescriptor(TypeDescriptor* descriptor) \
    { \
        descriptor->name = #T; \
        descriptor->size = sizeof(T); \
        \
        TypeFactoryImp::hook(#T, #C, []() -> void* { return new T(); }); \
    } \
	\
	const TypeDescriptor& T::getTypeDescriptor() const \
	{ \
		return T::s_typeDescriptor; \
	}

#define REFLECT_IMP(T) REFLECT_IMP_CATEGORY(T, Default)

#define REFLECT_IMP_CATEGORY_ALIAS(T, C, N) \
    TypeDescriptor T::s_typeDescriptor{T::registerTypeDescriptor}; \
    \
    void T::registerTypeDescriptor(TypeDescriptor* descriptor) \
    { \
        descriptor->name = N; \
        descriptor->size = sizeof(T); \
        \
        TypeFactoryImp::hook(#T, #C, []() -> void* { return new T(); }); \
    } \
	\
	const TypeDescriptor& T::getTypeDescriptor() const \
	{ \
		return T::s_typeDescriptor; \
	}

#define REFLECT_IMP_ALIAS(T, C, N) REFLECT_IMP_CATEGORY_ALIAS(T, Default, N)
};