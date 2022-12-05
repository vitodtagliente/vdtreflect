// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <map>
#include <string>
#include <vector>

#include "test_generated.h"

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
	PROPERTY() int c{ 14 };

	GENERATED_BODY()
};

ENUM()
enum class UserRole : int
{
	Guest,
	Admin
};

CLASS()
class User : public IType
{
public:
	PROPERTY() std::string name;
	PROPERTY() std::string surname;
	PROPERTY() int age;
	PROPERTY() float height;
	PROPERTY() double borntime;
	PROPERTY() char sex;
	PROPERTY() bool active;
	PROPERTY() UserRole role;
	PROPERTY() Foo foo;
	PROPERTY() Foo* foo_ptr;
	PROPERTY() std::vector<int> v;
	PROPERTY() std::map<std::string, Foo> m;
	PROPERTY() std::map<std::string, std::unique_ptr<Foo>> cm;
	PROPERTY() std::vector<Foo> foos;
	PROPERTY() std::vector<std::unique_ptr<Foo>> cfoos;

	GENERATED_BODY()
};

ENUM()
enum class ApplicationMode : int
{
	Editor = 0,
	Server = 1,
	Standalone = 2,
};