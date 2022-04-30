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

REFLECT_ENUM(AssetType, None, Image = 4, Sprite)

int main()
{
	cout << TypeDescriptor::get<Foo>().name << std::endl;
	for (const auto& name : TypeFactory::list("PAWN"))
	{
		cout << name << " ";
	}
	cout << endl;

	cout << enumToString(AssetType::None) << endl;
	cout << enumToString(AssetType::Image) << endl;

	AssetType type;

	if (stringToEnum<AssetType>("Image", type))
	{
		cout << static_cast<int>(type) << endl;
	}

	if (stringToEnum<AssetType>("Sprite", type))
	{
		cout << static_cast<int>(type) << endl;
	}

	if (stringToEnum<AssetType>("Foo", type))
	{
		cout << static_cast<int>(type) << endl;
	}

	return 0;
}