// Copyright (c) Vito Domenico Tagliente
// automatically generated by the compiler, do not modify
#include "test.h"

const char* EnumType<TestEnum>::name() { return "TestEnum"; }
const std::map<const char*, int>  EnumType<TestEnum>::values()
{
    static std::map<const char*, int> s_values{
        { "A", static_cast<int>(TestEnum::A) }, 
        { "B", static_cast<int>(TestEnum::B) }, 
    };
    return s_values;
}

const char* EnumType<ApplicationMode>::name() { return "ApplicationMode"; }
const std::map<const char*, int>  EnumType<ApplicationMode>::values()
{
    static std::map<const char*, int> s_values{
        { "Editor", static_cast<int>(ApplicationMode::Editor) }, 
        { "Server", static_cast<int>(ApplicationMode::Server) }, 
        { "Standalone", static_cast<int>(ApplicationMode::Standalone) }, 
    };
    return s_values;
}

const meta_t& Foo::getTypeMeta() const { return FooType::meta(); }
const char* Foo::getTypeName() const { return FooType::name(); }
const properties_t Foo::getTypeProperties() const { return FooType::properties(this); }
std::size_t Foo::getTypeSize() const { return FooType::size(); }

FooType::FooType()
{
    TypeFactoryRegister::load(FooType::name(), []() -> const meta_t& { return FooType::meta(); }, []() -> IType* { return FooType::instantiate(); });
}

Foo* const FooType::instantiate()
{
    return new Foo();
}

const meta_t& FooType::meta()
{
    static meta_t s_meta{
        std::make_pair("Category", "MyClass"),
        std::make_pair("Serializable", "true"),
    };
    return s_meta;
}

std::size_t FooType::size()
{
    return sizeof(Foo);
}

const char* FooType::name() { return "Foo"; }
properties_t FooType::properties(const Foo* const pointer)
{
    member_address_t origin = reinterpret_cast<member_address_t>(pointer);
    properties_t properties;
    properties.insert(std::make_pair<std::string, Property>("a", Property("a", NativeType::NT_int, "int", sizeof(int), origin + offsetof(Foo, a), {
        std::make_pair("JsonExport", "true"),
    })));
    properties.insert(std::make_pair<std::string, Property>("b", Property("b", NativeType::NT_int, "int", sizeof(int), origin + offsetof(Foo, b), {
    })));
    return properties;
}

FooType __foo_type;

const meta_t& Poo::getTypeMeta() const { return PooType::meta(); }
const char* Poo::getTypeName() const { return PooType::name(); }
const properties_t Poo::getTypeProperties() const { return PooType::properties(this); }
std::size_t Poo::getTypeSize() const { return PooType::size(); }

PooType::PooType()
{
    TypeFactoryRegister::load(PooType::name(), []() -> const meta_t& { return PooType::meta(); }, []() -> IType* { return PooType::instantiate(); });
}

Poo* const PooType::instantiate()
{
    return new Poo();
}

const meta_t& PooType::meta()
{
    static meta_t s_meta{
    };
    return s_meta;
}

std::size_t PooType::size()
{
    return sizeof(Poo);
}

const char* PooType::name() { return "Poo"; }
properties_t PooType::properties(const Poo* const pointer)
{
    member_address_t origin = reinterpret_cast<member_address_t>(pointer);
    properties_t properties = FooType::properties(pointer);
    properties.insert(std::make_pair<std::string, Property>("c", Property("c", NativeType::NT_int, "int", sizeof(int), origin + offsetof(Poo, c), {
    })));
    return properties;
}

PooType __poo_type;
