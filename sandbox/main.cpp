#include <iostream>
#include <vdtreflect/runtime.h>

#include "test.h"

using namespace std;

int main()
{
	cout << "Testing Foo's properties: " << endl;
	{
		Foo foo;
		for (const auto& [name, prop] : foo.type_properties())
		{
			cout << name << " " << prop.value<int>(&foo) << endl;
		}
	}

	cout << "Registered types: " << endl;
	{
		for (const auto& [name, def] : TypeFactory::list())
		{
			cout << name << ": ";
			IType* const type = std::get<1>(def)();
			cout << (type != nullptr ? "can instantiate" : "cannot instantiate") << endl;
		}
	}

	cout << "Testing enums: " << endl;
	{
		cout << enumToString(TestEnum::A) << endl;
	}

	cout << "Instantiating a Foo type" << endl;
	{
		Foo* foo = TypeFactory::instantiate<Foo>("Foo");
		for (const auto& [name, prop] : foo->type_properties())
		{
			cout << "property: " << name << ", value: " << prop.value<int>(foo) << endl;
		}
	}

	cout << "Instantiating a Poo type" << endl;
	{
		Poo* poo = TypeFactory::instantiate<Poo>("Poo");
		for (const auto& [name, prop] : poo->type_properties())
		{
			cout << "property: " << name << endl;
		}
	}
}