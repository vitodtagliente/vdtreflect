#include <iostream>
#include <vdtreflect/runtime.h>

#include "test.h"

using namespace std;

int main()
{
	cout << "Registered types: " << endl;
	{
		for (const auto& [name, def] : reflect::TypeFactory::list())
		{
			cout << name << ": ";
			reflect::IType* const type = std::get<1>(def)();
			cout << (type != nullptr ? "can instantiate" : "cannot instantiate") << endl;
		}
	}

	cout << "Testing Foo's properties: " << endl;
	{
		Foo foo;
		for (const auto& [name, prop] : foo.type_properties())
		{
			cout << name << " " << prop.value<int>(&foo) << endl;
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

	{
		Poo poo;
		poo.m_int = 17;
		std::string json = poo.to_json();
		poo.m_int = 0;
		poo.from_json(json);

		Foo foo;
	}
	
	{
		std::string str;
		std::string json;
		{
			Too too;
			too.types.push_back(std::make_unique<Foo>());
			too.types.push_back(std::make_unique<Poo>());
			str = too.to_string();
			json = too.to_json();
		}

		Too too1;
		too1.from_string(str);

		Too too;
		too.from_json(json);
		for (const auto& element : too.types)
		{
			cout << element->type_name() << endl;
		}
	}
}