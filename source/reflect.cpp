#include <vdtreflect/reflect.h>

namespace reflect
{
	std::map<std::string, std::set<std::string>>& TypeFactoryImp::categories()
	{
		static std::map<std::string, std::set<std::string>> s_categories;
		return s_categories;
	}

	std::map<std::string, factory_constructor_t>& TypeFactoryImp::data()
	{
		static std::map<std::string, factory_constructor_t> s_register;
		return s_register;
	}

	void TypeFactoryImp::hook(const std::string& name, const std::string& category, factory_constructor_t handler)

	{
		data().insert(std::make_pair(name, handler));

		const auto& it = categories().find(category);
		if (it != categories().end())
		{
			it->second.insert(name);
		}
		else
		{
			categories().insert(std::make_pair(category, std::set<std::string>{ name }));
		}
	}

	TypeDescriptor::TypeDescriptor(std::function<void(TypeDescriptor*)> init)
		: name()
		, size()
	{
		init(this);
	}

	void* TypeFactory::instantiate(const std::string& name)
	{
		const std::map<std::string, factory_constructor_t>& constructors = TypeFactoryImp::data();
		const auto& it = constructors.find(name);
		if (it != constructors.end())
		{
			return it->second();
		}
		return nullptr;
	}

	std::set<std::string> TypeFactory::list(const std::string& category)
	{
		const std::map<std::string, std::set<std::string>>& categories = TypeFactoryImp::categories();
		const auto& it = categories.find(category);
		if (it != categories.end())
		{
			return it->second;
		}
		return {};
	}
}