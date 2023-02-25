#include "type.h"

TypeCollection::~TypeCollection()
{
}

TypeClass* const TypeCollection::addClass(const std::string& name)
{
	TypeClass* const e = new TypeClass();
	m_entities.push_back(e);
	return e->name = name, e;
}

TypeClass* const TypeCollection::findClass(const std::string& name) const
{
	const auto& it = std::find_if(m_entities.begin(), m_entities.end(),
		[&name](const Type* elem) -> bool
		{
			return elem->name == name;
		}
	);

	if (it != m_entities.end())
	{
		return dynamic_cast<TypeClass*>(*it);
	}
	return nullptr;
}

TypeEnum* const TypeCollection::addEnum(const std::string& name)
{
	TypeEnum* const e = new TypeEnum();
	m_entities.push_back(e);
	return e->name = name, e;
}

TypeEnum* const TypeCollection::findEnum(const std::string& name) const
{
	const auto& it = std::find_if(m_entities.begin(), m_entities.end(),
		[&name](const Type* elem) -> bool
		{
			return elem->name == name;
		}
	);

	if (it != m_entities.end())
	{
		return dynamic_cast<TypeEnum*>(*it);
	}
	return nullptr;
}

bool TypeCollection::empty() const
{
	return m_entities.empty();
}

TypeEnum* const TypeEnum::addOption(const std::string& optionName)
{
	options.push_back(optionName);
	return this;
}

TypeClass* const TypeClass::addProperty(const std::string& memberName, const std::string& type, const meta_t& meta)
{
	Property prop;
	prop.name = memberName;
	prop.type = type;
	prop.meta = meta;

	properties.push_back(prop);
	return this;
}