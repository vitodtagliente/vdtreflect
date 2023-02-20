// Copyright (c) Vito Domenico Tagliente
// automatically generated by the compiler, do not modify
#pragma once

#include <vdtreflect/runtime.h>

template <>
struct Enum<enum class TestEnum> : RegisteredInEnumFactory<enum class TestEnum>
{
    static const char* const name();
    static const enum_values_t& values();
    
    static bool registered() { return value; };
};

template <>
struct Enum<enum class UserRole> : RegisteredInEnumFactory<enum class UserRole>
{
    static const char* const name();
    static const enum_values_t& values();
    
    static bool registered() { return value; };
};

template <>
struct Enum<enum class ApplicationMode> : RegisteredInEnumFactory<enum class ApplicationMode>
{
    static const char* const name();
    static const enum_values_t& values();
    
    static bool registered() { return value; };
};

struct __FooType : RegisteredInTypeFactory<__FooType>
{
    __FooType() = delete;

    static const TypeDefinition& type();
    static bool registered() { return value; };
};

struct __PooType : RegisteredInTypeFactory<__PooType>
{
    __PooType() = delete;

    static const TypeDefinition& type();
    static bool registered() { return value; };
};

struct __UserType : RegisteredInTypeFactory<__UserType>
{
    __UserType() = delete;

    static const TypeDefinition& type();
    static bool registered() { return value; };
};

