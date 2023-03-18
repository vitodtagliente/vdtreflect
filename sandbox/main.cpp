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

	{
		Foo foo;
		foo.a = 17;
		foo.b = 12;
		reflect::encoding::ByteBuffer buffer;
		reflect::encoding::OutputByteStream out(buffer);
		out << foo.a;
		out << foo.b;
		out << std::string("mondo");
		reflect::encoding::ByteBufferFile::save(buffer, "test.txt");

		Foo foo1;
		std::string str;
		reflect::encoding::ByteBufferFile::load(buffer, "test.txt");
		reflect::encoding::InputByteStream in(buffer);
		in >> foo.a;
		in >> foo.b;
		in >> str;
	}

}