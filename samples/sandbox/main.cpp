#include <iostream>
#include <vdtreflect/reflect.h>

using namespace std;
using namespace reflect;

struct Foo
{
	REFLECT()
};

REFLECT_IMP(Foo)

struct Pawn
{
};

struct Soldier : public Pawn
{
	REFLECT()
};

REFLECT_IMP_CATEGORY(Soldier, PAWN)

struct Knight : public Pawn
{
	REFLECT()
};

REFLECT_IMP_CATEGORY(Knight, PAWN)

int main()
{
	cout << TypeDescriptor::get<Foo>().name << std::endl;
	for (const auto& name : TypeFactory::list("PAWN"))
	{
		cout << name << " ";
	}
	cout << endl;
	return 0;
}