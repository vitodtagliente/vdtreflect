#include "type.h"

TypeClass* const TypeCollection::addClass(const std::string& name)
{
	m_entities.push_back(std::make_unique<TypeClass>());
	TypeClass* const e = dynamic_cast<TypeClass*>(m_entities.back().get());
	return e->name = name, e;
}

TypeEnum* const TypeCollection::addEnum(const std::string& name)
{
	m_entities.push_back(std::make_unique<TypeEnum>());
	TypeEnum* const e = dynamic_cast<TypeEnum*>(m_entities.back().get());
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