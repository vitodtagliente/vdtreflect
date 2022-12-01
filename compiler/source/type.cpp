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

TypeEnum* const TypeCollection::addEnum(const std::string& name)
{
	TypeEnum* const e = new TypeEnum();
	m_entities.push_back(e);
	return e->name = name, e;
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