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

const char* EnumType<UserRole>::name() { return "UserRole"; }
const std::map<const char*, int>  EnumType<UserRole>::values()
{
    static std::map<const char*, int> s_values{
        { "Guest", static_cast<int>(UserRole::Guest) }, 
        { "Admin", static_cast<int>(UserRole::Admin) }, 
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

const meta_t& Foo::getTypeMeta() const { return FooType::type().meta; }
const std::string& Foo::getTypeName() const { return FooType::type().name; }
const properties_t Foo::getTypeProperties() const {
    member_address_t origin = reinterpret_cast<member_address_t>(this);
    properties_t properties;
    properties.insert(std::make_pair<std::string, Property>("a", Property("a", Property::TypeDescriptor("int", Property::Type::T_int, Property::DecoratorType::D_normalized, {}), sizeof(int), origin + offsetof(Foo, a), {
        std::make_pair("JsonExport", "true"),
    })));
    properties.insert(std::make_pair<std::string, Property>("b", Property("b", Property::TypeDescriptor("int", Property::Type::T_int, Property::DecoratorType::D_normalized, {}), sizeof(int), origin + offsetof(Foo, b), {
    })));
    return properties;
}
std::size_t Foo::getTypeSize() const { return FooType::type().size; }

const TypeDefinition& FooType::type()
{
    static const TypeDefinition s_typeDefinition([]() -> Type* { return new Foo(); }, "Foo", {
        std::make_pair("Category", "MyClass"),
        std::make_pair("Serializable", "true"),
    }, sizeof(Foo));
    return s_typeDefinition;
}
const meta_t& Poo::getTypeMeta() const { return PooType::type().meta; }
const std::string& Poo::getTypeName() const { return PooType::type().name; }
const properties_t Poo::getTypeProperties() const {
    member_address_t origin = reinterpret_cast<member_address_t>(this);
    properties_t properties = Foo::getTypeProperties();
    properties.insert(std::make_pair<std::string, Property>("c", Property("c", Property::TypeDescriptor("int", Property::Type::T_int, Property::DecoratorType::D_normalized, {}), sizeof(int), origin + offsetof(Poo, c), {
    })));
    return properties;
}
std::size_t Poo::getTypeSize() const { return PooType::type().size; }

const TypeDefinition& PooType::type()
{
    static const TypeDefinition s_typeDefinition([]() -> Type* { return new Poo(); }, "Poo", {
    }, sizeof(Poo));
    return s_typeDefinition;
}
const meta_t& User::getTypeMeta() const { return UserType::type().meta; }
const std::string& User::getTypeName() const { return UserType::type().name; }
const properties_t User::getTypeProperties() const {
    member_address_t origin = reinterpret_cast<member_address_t>(this);
    properties_t properties;
    properties.insert(std::make_pair<std::string, Property>("name", Property("name", Property::TypeDescriptor("std::string", Property::Type::T_container_string, Property::DecoratorType::D_normalized, {}), sizeof(std::string), origin + offsetof(User, name), {
    })));
    properties.insert(std::make_pair<std::string, Property>("surname", Property("surname", Property::TypeDescriptor("std::string", Property::Type::T_container_string, Property::DecoratorType::D_normalized, {}), sizeof(std::string), origin + offsetof(User, surname), {
    })));
    properties.insert(std::make_pair<std::string, Property>("age", Property("age", Property::TypeDescriptor("int", Property::Type::T_int, Property::DecoratorType::D_normalized, {}), sizeof(int), origin + offsetof(User, age), {
    })));
    properties.insert(std::make_pair<std::string, Property>("height", Property("height", Property::TypeDescriptor("float", Property::Type::T_float, Property::DecoratorType::D_normalized, {}), sizeof(float), origin + offsetof(User, height), {
    })));
    properties.insert(std::make_pair<std::string, Property>("borntime", Property("borntime", Property::TypeDescriptor("double", Property::Type::T_double, Property::DecoratorType::D_normalized, {}), sizeof(double), origin + offsetof(User, borntime), {
    })));
    properties.insert(std::make_pair<std::string, Property>("sex", Property("sex", Property::TypeDescriptor("char", Property::Type::T_char, Property::DecoratorType::D_normalized, {}), sizeof(char), origin + offsetof(User, sex), {
    })));
    properties.insert(std::make_pair<std::string, Property>("active", Property("active", Property::TypeDescriptor("bool", Property::Type::T_bool, Property::DecoratorType::D_normalized, {}), sizeof(bool), origin + offsetof(User, active), {
    })));
    properties.insert(std::make_pair<std::string, Property>("role", Property("role", Property::TypeDescriptor("UserRole", Property::Type::T_custom_enum, Property::DecoratorType::D_normalized, {}), sizeof(UserRole), origin + offsetof(User, role), {
    })));
    properties.insert(std::make_pair<std::string, Property>("foo", Property("foo", Property::TypeDescriptor("Foo", Property::Type::T_custom_type, Property::DecoratorType::D_normalized, {}), sizeof(Foo), origin + offsetof(User, foo), {
    })));
    properties.insert(std::make_pair<std::string, Property>("foo_ptr", Property("foo_ptr", Property::TypeDescriptor("Foo*", Property::Type::T_custom_type, Property::DecoratorType::D_normalized, {}), sizeof(Foo*), origin + offsetof(User, foo_ptr), {
    })));
    properties.insert(std::make_pair<std::string, Property>("v", Property("v", Property::TypeDescriptor("std::vector<int>", Property::Type::T_container_vector, Property::DecoratorType::D_normalized, {Property::TypeDescriptor("int", Property::Type::T_int, Property::DecoratorType::D_normalized, {})}), sizeof(std::vector<int>), origin + offsetof(User, v), {
    })));
    properties.insert(std::make_pair<std::string, Property>("m", Property("m", Property::TypeDescriptor("std::map<std::string, Foo>", Property::Type::T_container_map, Property::DecoratorType::D_normalized, {Property::TypeDescriptor("std::string", Property::Type::T_container_string, Property::DecoratorType::D_normalized, {}), Property::TypeDescriptor("Foo", Property::Type::T_custom_type, Property::DecoratorType::D_normalized, {})}), sizeof(std::map<std::string, Foo>), origin + offsetof(User, m), {
    })));
    properties.insert(std::make_pair<std::string, Property>("cm", Property("cm", Property::TypeDescriptor("std::map<std::string, std::unique_ptr<Foo>>", Property::Type::T_container_map, Property::DecoratorType::D_normalized, {Property::TypeDescriptor("std::string", Property::Type::T_container_string, Property::DecoratorType::D_normalized, {}), Property::TypeDescriptor("Foo", Property::Type::T_custom_type, Property::DecoratorType::D_unique_ptr, {})}), sizeof(std::map<std::string, std::unique_ptr<Foo>>), origin + offsetof(User, cm), {
    })));
    properties.insert(std::make_pair<std::string, Property>("foos", Property("foos", Property::TypeDescriptor("std::vector<Foo>", Property::Type::T_container_vector, Property::DecoratorType::D_normalized, {Property::TypeDescriptor("Foo", Property::Type::T_custom_type, Property::DecoratorType::D_normalized, {})}), sizeof(std::vector<Foo>), origin + offsetof(User, foos), {
    })));
    properties.insert(std::make_pair<std::string, Property>("cfoos", Property("cfoos", Property::TypeDescriptor("std::vector<std::unique_ptr<Foo>>", Property::Type::T_container_vector, Property::DecoratorType::D_normalized, {Property::TypeDescriptor("Foo", Property::Type::T_custom_type, Property::DecoratorType::D_unique_ptr, {})}), sizeof(std::vector<std::unique_ptr<Foo>>), origin + offsetof(User, cfoos), {
    })));
    return properties;
}
std::size_t User::getTypeSize() const { return UserType::type().size; }

const TypeDefinition& UserType::type()
{
    static const TypeDefinition s_typeDefinition([]() -> Type* { return new User(); }, "User", {
    }, sizeof(User));
    return s_typeDefinition;
}
