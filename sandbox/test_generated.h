// Copyright (c) Vito Domenico Tagliente
// automatically generated by the compiler, do not modify
#pragma once

#include <vdtreflect/runtime.h>

template <>
struct EnumType<enum class TestEnum>
{
    static const char* name();
    static const std::map<const char*, int> values();
};

template <>
struct EnumType<enum class ApplicationMode>
{
    static const char* name();
    static const std::map<const char*, int> values();
};

struct FooType
{
    FooType();

    static const meta_t& meta();
    static const char* name();
    static properties_t properties(const class Foo* const pointer);
    static class Foo* const instantiate();
    static std::size_t size();
};
extern FooType __foo_type;

struct PooType
{
    PooType();

    static const meta_t& meta();
    static const char* name();
    static properties_t properties(const class Poo* const pointer);
    static class Poo* const instantiate();
    static std::size_t size();
};
extern PooType __poo_type;
