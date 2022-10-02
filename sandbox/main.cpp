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
	Foo foo;
	test(&foo);

	Poo poo;
	test(&poo);

	cout << enumToString(TestEnum::A) << endl;

	const Foo* const f = TypeFactory::instantiate<Foo>("Foo");

	for (const std::string& typeName : TypeFactory::list("Category", "MyClass"))
	{
		cout << typeName << endl;
	}
}