// Copyright (c) Vito Domenico Tagliente
#pragma once

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
	PROPERTY(IsEnum) UserRole role;
	PROPERTY(IsType) Foo foo;
	PROPERTY(IsType) Foo* foo_ptr;

	GENERATED_BODY()
};

ENUM()
enum class ApplicationMode : int
{
	Editor = 0,
	Server = 1,
	Standalone = 2,
};