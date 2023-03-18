// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <map>
#include <string>
#include <vector>

#include "test_generated.h"

using namespace reflect;

ENUM(Category = MyEnums)
enum class TestEnum
{
	A,
	B = 2
};

CLASS(Category = MyClass, Serializable = true)
class Foo : public IType
{
public:
	PROPERTY(JsonExport = true) int a{ 4 };
	PROPERTY() int b = 5;

	GENERATED_BODY()
};

CLASS()
class Poo : public Foo
{
public:
	PROPERTY() int c = 15;
	PROPERTY() std::vector<int> numbers;
	PROPERTY() std::map<std::string, int> dictionary;
	PROPERTY() std::tuple<int, float, bool, double> tuple;
	PROPERTY() std::map<std::string, std::vector<int>> power_dictionary;
	PROPERTY() TestEnum e;
	PROPERTY() Foo type;

	GENERATED_BODY()
};

CLASS()
struct Too : public Foo
{
public:
	PROPERTY() int c = 14;

	GENERATED_BODY()
};

ENUM()
enum class ApplicationMode : int
{
	Editor = 0,
	Server = 1,
	Standalone = 2,
};