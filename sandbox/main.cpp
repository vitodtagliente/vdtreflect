#include <iostream>
#include <vdtreflect/runtime.h>

#include "test.h"

using namespace std;

void test(const IType* type)
{
	cout << type->getTypeName() << endl;
	for (const auto& [name, field] : type->getTypeProperties())
	{
		cout << name << " = " << field.value<int>() << endl;
	}

}

int main()
{
	{
		Foo foo;
		test(&foo);

		Poo poo;
		test(&poo);

		cout << enumToString(TestEnum::A) << endl;

		const Foo* const f = TypeFactory::instantiate<Foo>("Foo");

		for (const std::string& typeName : TypeFactory::list())
		{
			cout << typeName << endl;
		}
	}

	User user;
	// access IType properties
	for (const auto& [name, property] : user.getTypeProperties())
	{
		if (name == "foo")
		{
			Foo& foo = property.value<Foo>();
			foo.b = 6;

			IType* _foo = &property.value<IType>();
			test(_foo);
		}
	}

	// access IType vectors
	for (const auto& [name, property] : user.getTypeProperties())
	{
		if (name == "cfoos")
		{
			std::vector<unique_ptr<Foo>>& foos = property.value<std::vector<unique_ptr<Foo>>>();
			Foo* foo = new Foo();
			foo->a = 13;
			foo->b = 56;
			foos.push_back(std::unique_ptr<Foo>(foo));

			auto& v = property.value<std::vector<unique_ptr<IType>>>();
			for (auto& element : v)
			{
				test(element.get());
			}
		}
	}

	// access IType maps
	for (const auto& [name, property] : user.getTypeProperties())
	{
		break;
		if (name == "m")
		{
			std::map<std::string, Foo>& m = property.value<std::map<std::string, Foo>>();
			Foo foo;
			foo.a = 13;
			foo.b = 56;
			m.insert(std::make_pair("foo", foo));

			for (auto& [name, element] : property.value<std::map<std::string, size_t>>())
			{
				IType* t = reinterpret_cast<IType*>(&element);
				test(t);
			}
		}
	}

	std::vector<Foo> foos;
	{
		Foo foo;
		foo.a = 13;
		foo.b = 56;
		foos.push_back(foo);
	}
	{
		Foo foo;
		foo.a = 24;
		foo.b = 12;
		foos.push_back(foo);
	}
	size_t begin = reinterpret_cast<size_t>(&foos[0]);
	size_t end = reinterpret_cast<size_t>(&foos[foos.size() - 1]);

	size_t elemSize = TypeFactory::instantiate("Foo")->getTypeSize();
	size_t diff = (end - begin);
	size_t count = diff / elemSize;

	Foo* foo1 = reinterpret_cast<Foo*>(begin);
	Foo* foo2 = reinterpret_cast<Foo*>(begin + elemSize);

	

	if (true)
	{

	}
}